# PokéTempo / PokéTime — 0.1.1
Relógio independente para Pokémon Pokopia no **Recompia para Windows**, por **DigoTK**. Código aberto sob MIT.

O mod [Date and Time](https://pokopia-native.vercel.app/mods/date-time) já cobre a proposta central. Veja a [comparação e situação da publicação](publication/COMPARISON.md); não apresentamos a ideia como inédita.

[English](README.md) · [Configuração](docs/CONFIGURATION.md) · [Desenvolvimento](docs/DEVELOPMENT.md) · [Validação](VALIDACAO.md)

[Downloads](https://github.com/DigoTK/poketempo/releases) · [Relatar problema](https://github.com/DigoTK/poketempo/issues)

**O padrão é uma hora real avançar quatro horas no jogo: um dia completo dura seis horas reais.** Configure no launcher ou pelo menu F1 existente. Node.js é necessário somente para o painel opcional.

## Compatibilidade
Avanço do relógio testado em Recompia 0.13.0, Windows x64, jogo **2.0.1**, Modtopia ABI 0.6. Loader aceito: >=0.6.0 e <1.0.0. Outros hosts, incluindo Pokopia Native, precisam de testes próprios. Este pacote não é para console ou emulador. Multiplayer não foi validado.

## Instalação recomendada
1. Feche o jogo e faça backup do save e do estado existente do relógio do mod.
2. Coloque `poketempo-0.1.1.pkmod` na pasta `mods` do host. Mantenha apenas uma cópia do PokéTempo: retire versões antigas dessa pasta após fazer backup.
3. Habilite PokéTempo em **Mods** no launcher, autorize seu código quando solicitado e execute **Set up** novamente se o launcher pedir.
4. Escolha **4** em **Horas no jogo por hora real** e inicie.
5. Durante a partida, use **F1 → Mods → PokéTempo** para velocidade e pausa.

Os rótulos nativos incluem português e inglês; o idioma da interface ao redor depende do host. A data inicial é um campo de texto: edite no launcher com o jogo fechado. No F1 atual, campos de texto são apenas exibidos.

| Opção | Padrão | Aplicação |
| --- | --- | --- |
| Velocidade: 0,5–120×, passos de 0,5 | 4× | Launcher ou F1; ao vivo |
| Congelar horário | Desligado | Launcher ou F1; ao vivo |
| Avançar com jogo fechado | Desligado | Próxima abertura |
| Data/hora inicial | Vazio | Launcher ou painel; próxima abertura |

| Velocidade | Duração real de um dia no jogo |
| --- | --- |
| 1× | 24 horas |
| 4× | 6 horas |
| 20× | 72 minutos |
| 24× | 60 minutos |

A proporção vale enquanto o jogo atualiza o gerenciador de tempo. Carregamentos, interrupções e pausas podem alterar a duração observada. O mod atua no calendário/tempo, sem multiplicar a física global. Estações do ano e clima não estão implementados.

## Painel opcional PT-BR / EN
Extraia `PokeTempo-0.1.1-Windows.zip`, instale Node.js 22+ e abra `Abrir painel.cmd`. Na primeira vez, indique a pasta Recompia que contém `pkp.exe` e `pkp_rt.dll`. Use `Configurar painel.cmd` para trocar a pasta.

O painel abre em `http://127.0.0.1:43821`. Escolha PT / EN. Não há npm install nem conta. Ele grava a mesma configuração do mod, somente com o jogo fechado, e faz backup da configuração anterior. Não precisa permanecer aberto durante a partida.

Se instalou o .pkmod, basta configurar o painel. `Instalar.cmd` é uma alternativa de instalação por pasta; não combine os dois métodos. O instalador também copia a pasta padrão `save` para backup. Saves em locais personalizados exigem backup próprio.

## Persistência e remoção
Sem data inicial, a primeira execução usa o horário retornado pelo jogo; depois retoma o próprio horário salvo. O Windows não tem seu relógio alterado.

O estado é gravado a cada dez segundos e ao salvar o jogo, em `config/poketempo/clock-<perfil>.txt`. Uma saída abrupta pode perder o intervalo recente. O caminho da pasta de save determina qual relógio é usado.

Alterar o texto da data inicial cria um novo ponto de partida na próxima abertura. Manter o mesmo texto retoma o estado salvo. No modo offline, o intervalo real positivo desde a última gravação é multiplicado pela velocidade escolhida na abertura, exceto quando congelado.

Para desativar, desligue o mod no launcher e conclua a preparação solicitada. Para remover, feche o jogo e retire o pacote/pasta. Preserve configuração e estado para reinstalação. Mudar datas ou retornar ao relógio original pode afetar temporizadores; experimente em um backup.

## Estado e créditos
A DLL real passou em testes com host simulado, e DigoTK confirmou o avanço acelerado em jogo. Iluminação, eventos, viradas de dia e multiplayer ainda precisam de validação sistemática. Veja [VALIDACAO.md](VALIDACAO.md).

**DigoTK:** direção e testes em jogo. **OpenAI Codex:** maior parte do código, testes, interface e documentação. **The Modtopia contributors:** SDK MIT.
[Créditos](THIRD_PARTY_NOTICES.md) · [Licença](LICENSE) · [Contribuições](CONTRIBUTING.md) · [Histórico](CHANGELOG.md) · [Publicação](publication/README.pt-BR.md).

Sem arquivos, chaves, código extraído, arte ou áudio do jogo. Sem telemetria ou acesso externo; o painel atende apenas localhost.

