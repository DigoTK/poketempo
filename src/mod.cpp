#include <windows.h>
#include <modtopia/modtopia.h>
#include "clock_core.h"
#include <mutex>
#include <string>

MT_DECLARE_MOD()
namespace {
const MtHost *host;
MtMod *mod;
std::mutex lock;
double now_seconds=0, rate=4;
bool ready=false, paused=false, offline=false;
std::string origin;
std::wstring state_path;
ULONGLONG last_write=0;
unsigned long long updates=0, clock_reads=0;

std::wstring wide(const char *s) {
    int n=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s,-1,nullptr,0);
    if(!n) return {};
    std::wstring w(n,L'\0'); MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s,-1,w.data(),n);
    w.pop_back(); return w;
}
void settings() {
    rate=host->config_number(mod,"speed");
    if(!std::isfinite(rate) || rate<0.1 || rate>120) rate=4;
    paused=host->config_bool(mod,"paused")!=0;
    offline=host->config_bool(mod,"offline")!=0;
}
void changed(MtMod*,const char*,void*) { std::lock_guard<std::mutex> guard(lock); settings(); }
void persist() {
    if(!ready) return;
    auto temporary=state_path+L".tmp";
    FILE *f=_wfopen(temporary.c_str(),L"wb");
    if(!f) { host->log(mod,MT_LOG_ERROR,"Cannot write clock state."); return; }
    int n=fprintf(f,"POKETIME1\n%.9f\n%lld\n%s\n",now_seconds,(long long)time(nullptr),origin.c_str());
    int close_result=fclose(f);
    if(n<0 || close_result || !MoveFileExW(temporary.c_str(),state_path.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))
        host->log(mod,MT_LOG_ERROR,"Clock state could not be committed; previous state retained.");
    last_write=GetTickCount64();
}
int saved(MtMod*,const MtEvent*,void*) { std::lock_guard<std::mutex> guard(lock); persist(); return MT_EVENT_CONTINUE; }
int frame(MtMod*,const MtEvent*,void*) {
    std::lock_guard<std::mutex> guard(lock);
    if(ready && GetTickCount64()-last_write>=10000) persist();
    return MT_EVENT_CONTINUE;
}
int clock_hook(MtHookCtx *ctx,void*) {
    const auto address=host->reg_get(ctx,MT_REG_X0);
    // Call the original only for the first seed. Never hold the mutex across a guest call.
    bool needs_seed;
    { std::lock_guard<std::mutex> guard(lock); needs_seed=!ready; }
    if(needs_seed) {
        if(host->call_original(ctx)!=MT_OK) return MT_CONTINUE;
        int date[6]; double seed;
        if(host->mem_read(address,date,sizeof date)!=MT_OK || !poketime::encode(date,seed)) return MT_HANDLED;
        std::lock_guard<std::mutex> guard(lock);
        if(!ready) { now_seconds=seed; ready=true; }
    }
    std::lock_guard<std::mutex> guard(lock);
    int date[6];
    if(!poketime::decode(now_seconds,date) || host->mem_write(address,date,sizeof date)!=MT_OK) {
        host->log(mod,MT_LOG_ERROR,"Clock write failed; using the original function."); return MT_CONTINUE;
    }
    if(++clock_reads==1) mt_logf(host,mod,MT_LOG_INFO,"Clock connected: %04d-%02d-%02d %02d:%02d:%02d",date[0],date[1],date[2],date[3],date[4],date[5]);
    return MT_HANDLED;
}
int update_hook(MtHookCtx *ctx,void*) {
    std::lock_guard<std::mutex> guard(lock);
    if(!ready) return MT_CONTINUE;
    float dt=mt_vreg_f32(host,ctx,0);
    if(!std::isfinite(dt) || dt<0 || dt>2) return MT_CONTINUE;
    double next=poketime::step(now_seconds,dt,rate,paused);
    float scaled=static_cast<float>(next-now_seconds);
    if(mt_vreg_set_f32(host,ctx,0,scaled)==MT_OK) now_seconds=next;
    if(++updates==1) mt_logf(host,mod,MT_LOG_INFO,"TimeMgr connected: %.2fx, paused=%d",rate,paused);
    return MT_CONTINUE;
}
}
MT_EXTERN_C MT_EXPORT int mod_init(const MtHost *h,MtMod *m) {
    host=h; mod=m;
    if(!MT_HOST_HAS(h,data_dir) || !h->data_dir || !h->save_dir || strcmp(h->game_version,"2.0.1")) return MT_ERR_VERSION;
    settings();
    const char *start=h->config_string(m,"start_at");
    origin=start ? start : "";
    if(!origin.empty() && !poketime::parse(origin.c_str(),now_seconds)) {
        h->log(m,MT_LOG_ERROR,"Invalid start date. Expected YYYY-MM-DD HH:MM:SS (1970..2099)."); return MT_ERR;
    }
    ready=!origin.empty();
    // Separate clocks for separate save folders / profiles.
    unsigned long long hash=14695981039346656037ull;
    const char *save=h->save_dir(m); if(!save) return MT_ERR;
    for(const unsigned char *p=(const unsigned char*)save;*p;++p) {hash^=*p;hash*=1099511628211ull;}
    const char *folder=h->data_dir(m); if(!folder) return MT_ERR;
    wchar_t name[80]; swprintf(name,80,L"/clock-%016llx.txt",hash);
    state_path=wide(folder)+name;
    FILE *f=_wfopen(state_path.c_str(),L"rb");
    if(f) {
        char magic[32]{},stored_origin[128]{}; double stored=0; long long wall=0;
        bool ok=fgets(magic,sizeof magic,f) && !strcmp(magic,"POKETIME1\n") && fscanf(f,"%lf\n%lld\n",&stored,&wall)==2;
        if(ok && fgets(stored_origin,sizeof stored_origin,f)) stored_origin[strcspn(stored_origin,"\r\n")]=0;
        fclose(f);
        if(!ok || !poketime::valid(stored) || wall<0) { h->log(m,MT_LOG_ERROR,"Invalid clock state. Keep a copy and restore it before loading this mod."); return MT_ERR; }
        if(origin==stored_origin) {
            now_seconds=stored; ready=true;
            if(offline && !paused) {
                double elapsed=difftime(time(nullptr),static_cast<time_t>(wall));
                if(elapsed>0) now_seconds=fmin(poketime::max_time,now_seconds+elapsed*rate);
            }
        }
    }
    if(h->hook(m,"calendar",clock_hook,nullptr)!=MT_OK || h->hook(m,"advance",update_hook,nullptr)!=MT_OK) return MT_ERR;
    if(h->on_config_changed(m,changed,nullptr)!=MT_OK || h->subscribe(m,"SaveWritten",MT_PRIORITY_NORMAL,saved,nullptr)!=MT_OK ||
       h->subscribe(m,"FrameStart",MT_PRIORITY_LOW,frame,nullptr)!=MT_OK) return MT_ERR;
    last_write=GetTickCount64();
    mt_logf(h,m,MT_LOG_INFO,"PokeTempo ready: %.2fx, full day %.2f real hours. Offline=%d",rate,24/rate,offline);
    return MT_OK;
}
