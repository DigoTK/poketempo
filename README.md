# PokéTempo / PokéTime — 0.1.1
Independent time for Pokémon Pokopia on **Windows Recompia**, by **DigoTK**. Open source under MIT.

The existing [Date and Time](https://pokopia-native.vercel.app/mods/date-time) mod covers the same core use case. See [comparison and publication status](publication/COMPARISON.md); this project does not claim novelty.

[Português do Brasil](README.pt-BR.md) · [Configuration](docs/CONFIGURATION.md) · [Development](docs/DEVELOPMENT.md) · [Validation](VALIDACAO.md)

[Downloads](https://github.com/DigoTK/poketempo/releases) · [Report an issue](https://github.com/DigoTK/poketempo/issues)

**One real hour advances four game hours by default. A full game day takes six real hours.** Configure in the launcher or the existing F1 Mods menu. Node.js is needed only for the optional browser panel.

## Compatibility
Clock progression tested on Recompia 0.13.0, Windows x64, game **2.0.1**, Modtopia ABI 0.6. Manifest loader range: >=0.6.0 and <1.0.0. Other hosts, including Pokopia Native, need separate testing. This package is not a console or emulator mod. Multiplayer is unvalidated.

## Native installation — recommended
1. Close the game and back up your save and existing mod clock state.
2. Put `poketempo-0.1.1.pkmod` in the host's `mods` folder. Keep only one PokéTempo installation; move older packages/folders out after backing them up.
3. Enable PokéTempo in launcher **Mods**, allow its code when prompted, and run **Set up** again if requested.
4. Set **Game hours per real hour** to **4**, then launch.
5. During play, use **F1 → Mods → PokéTempo** for speed and freeze controls.

The native labels include Portuguese and English; the host controls its surrounding language. Initial date is a text field: edit it in the launcher while the game is closed. Current F1 text controls are read-only.

| Setting | Default | Application |
| --- | --- | --- |
| Speed: 0.5–120×, steps of 0.5 | 4× | Launcher or F1; live |
| Freeze clock | Off | Launcher or F1; live |
| Advance while closed | Off | Next launch |
| Initial date/time | Empty | Launcher or panel; next launch |

| Speed | Full game day |
| --- | --- |
| 1× | 24 real hours |
| 4× | 6 real hours |
| 20× | 72 real minutes |
| 24× | 60 real minutes |

Ratios apply while the game's time updates run. Loading, interrupted updates and pauses may affect wall-clock observations. The mod targets calendar/time updates, not global physics. Seasons and weather controls are not implemented.

## Optional PT-BR / English panel
Extract `PokeTempo-0.1.1-Windows.zip`, install Node.js 22+ and open `Abrir painel.cmd`. On first launch, select the Recompia folder containing `pkp.exe` and `pkp_rt.dll`. Use `Configurar painel.cmd` to change this later.

The panel opens at `http://127.0.0.1:43821`. Select PT / EN. No npm install or account is needed. It saves the same mod settings only with the game closed and backs up the previous configuration. It need not remain open during play.

If you installed the .pkmod, configure the panel only. `Instalar.cmd` is an alternative folder installer, not an additional step; it copies the bundled mod and backs up the default `save` folder. Back up custom save locations separately.

## Persistence and removal
Without an initial date, the first run seeds from the game clock, then resumes its own saved time. Windows time is unchanged. State is written every ten seconds and on game saves under `config/poketempo/clock-<profile>.txt`. An abrupt exit may lose the latest interval. Save-folder paths select separate clock files.

Changing the initial-date string starts a new timeline at next launch; keeping it unchanged resumes persisted time. Offline mode multiplies positive real elapsed time since the last checkpoint by the speed selected at launch, unless frozen.

Disable in the launcher and complete requested preparation. To remove, close the game and remove the mod package/folder. Preserve settings and clock state for reinstalling. Date changes or returning to the original clock may affect timers; experiment with a backup.

## Status and credits
The actual DLL passed simulated-host tests; DigoTK confirmed accelerated time in gameplay. Systematic lighting, events, day-boundary and multiplayer tests remain outstanding. See [validation](VALIDACAO.md).

**DigoTK:** project direction and gameplay testing. **OpenAI Codex:** most implementation, tests, UI and documentation. **The Modtopia contributors:** MIT SDK. [Credits](THIRD_PARTY_NOTICES.md) · [MIT license](LICENSE) · [Contributing](CONTRIBUTING.md) · [Changelog](CHANGELOG.md) · [Publication kit](publication/README.pt-BR.md).

No game files, keys, extracted game code, artwork or audio are distributed. No telemetry or external network requests; the panel serves localhost.

