# Validation / Validação
Updated 2026-11-13.

## Automated evidence
C++ calendar tests cover 4× progression, pause, invalid inputs, leap days, year rollover and calendar bounds. A simulated Modtopia host loaded the actual Windows DLL and verified both hooks, calendar writes, scaled delta, unrelated-register preservation and state resumed by a second process.

One simulated hour at 60 updates/second advances approximately four game hours, with less than one second of float-delta rounding error. Node tests exercise configuration, backups, blocking writes during gameplay and activation diagnostics. Version 0.1.1 adds native F1 slider access checks for exact 4× and 20×. Packaging checks integrity and exclusions.

## Gameplay observation
DigoTK confirmed that accelerated time worked after enabling and preparing the mod. The session log independently showed initialization and both clock hooks executing. Tested environment: Recompia 0.13.0, Windows x64, game 2.0.1.

The user mentioned 20×; a connection log recorded 24× at capture time. This confirms accelerated progression, not a measured 20× wall-clock accuracy test. The first failed attempt had the mod disabled and hooks unprepared.

Release 0.1.1 retains the gameplay-tested 0.1.0 DLL:
`EF402417BCEB701E62C5E3E7BA13A43E119A3D87CD7D20E8DD9C0CA0271EC045`.
Changes concern metadata, panel validation and distribution/docs. The revised F1 range is checked against host-source behavior; it has not been operated in another gameplay session.

## Still unvalidated
Systematic lighting/day transitions, events, construction timers, long sessions, loading/menus, multiplayer and other hosts. Seasons/weather are not implemented; no console/emulator build exists.

Before wider release, test on a backup save: ten uninterrupted real minutes at 4× (~40 game minutes), freeze/resume, restarts, offline on/off, day boundary, events and new F1 selection. Record actual observations; a panel animation is not evidence.

## Português
DigoTK confirmou o avanço acelerado em jogo, e o log mostrou as duas funções conectadas. A DLL também passou em testes automatizados. A revisão 0.1.1 mantém essa DLL e melhora configuração/distribuição. Eventos, iluminação e outros ambientes ainda precisam de validação sistemática.

