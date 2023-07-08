---
title: "EOS Practicum — OS Level Programming in C++"
keywords: [EOS]
---

# EOS Practicum — OS Level Programming in C++

Deze opdracht is individueel. De opdracht bestaat uit drie delen. Het eerste deel is het maken van een simpele utility die je als deel van een pipeline kan inzetten, bij het tweede deel gebruik je system calls om een basis-shell te implementeren. Je kunt de code op je normale systeem of op Linux (bijvoorbeeld in de VM) schrijven, maar voor het compileren en uitvoeren moet je sowieso Linux gebruiken. Voor het op Linux compilen van je code gebruik je `g++`, bijvoorbeeld `g++ file.cc -o file`. Je kunt ook de meegeleverde `Makefile` gebruiken door `make` te typen. Deze compileert beide programma's (ervan uitgaande dat je niets hernoemd of verplaatst hebt). Als dit goed compileert kun je dit daarna starten met `./file`. Met `make test` kun je vervolgens de test-scripts `testShell.sh` en `testTool.sh` draaien om te zien of je werk doet wat het moet doen. Als de tests goed lijken te werken lever je je code in via een **permalink naar een commit op GitHub**. Dit is een link die naar een specifieke versie verwijst, de versie die je inlevert. Als je je code naar GitHub gepusht hebt (dit kan met `git add [BESTANDEN]` gevolgd door `git commit -m "[COMMIT MESSAGE]"` en `git push`) kun je op GitHub al je bestanden zien in de laatste "commit". Door op `y` te drukken verandert de URL naar een permanente link naar deze versie; dit is wat je in Canvas moet inleveren.

## Opdracht 0 — Command Line Tools
Tijdens het vorige practicum  hebben we uitvoerig gebruik gemaakt van command line utilities op Linux. Deze programma's volgen allemaal de Unix filosofie: doe één taak, maar doe die goed. De reden dat dit zo krachtig is, is dat de kleine programma's gecombineerd kunnen worden om ingewikkelde problemen te automatiseren. Dit noemen we compositie, en kunnen we realiseren met behulp van standaard-invoer en standaard-uitvoer.
Maak een simpele utility die tekst-invoer leest vanuit `std::cin`, hier iets mee doet, en het resultaat uitvoert naar `std::cout`. Geef de utility (teminste) een argument dat de werking van de utility beïnvloed. Het maakt niet uit wat er met de input gebeurt, zolang deze (1) herkenbaar en afhankelijk van het argument verandert, en (2) de utility voor de hele invoer werkt (dus tot aan de EOF). **Anders dan de tekst die je utility gaat bewerken maak je geen gebruik van de command-line invoer; anders dan de bewerkte tekst lever je geen command-line uitvoer.** Eventuele extra output mag teruggegeven worden met behulp van `std::cerr` of `std::clog` (whichever meer van toepassing is). Alle extra informatie die de gebruiker moet geven gaat via command line argumenten. Zie de afbeelding hieronder voor een schematische weergave van een tool `caesar` die in een pipeline gecombineerd wordt met `ls` en `less`. Compile je utility, en laat zien dat deze te combineren is met andere utilities doormiddel van pipes en IO redirection. Je kan het bestand `tool.cc` uit [deze repository](https://github.com/HU-TI-V1EOS/V1EOS-practica) als beginpunt gebruiken.

![](0_tool_schematisch.png)

### Enkele suggesties
- Een [Caesarcijfer](https://nl.wikipedia.org/wiki/Caesarcijfer) waarbij het aantal plaatsen met een command-line argument kan worden aangegeven.
- Een vertaler voor [pig-latin](https://nl.wikipedia.org/wiki/Pig_Latin), waarbij de uitgang "-ay" met een argument aangepast kan worden.
- Een teller die telt hoe vaak ieder woord of karakter dat als argument wordt meegegeven voorkomt in de input.

## Opdracht 1 — Build Your Own Shell
Bij het programmeren, zelfs in een low-level taal als C++, nemen we vaak een hoop functionaliteit voor lief. We kunnen bijvoorbeeld bestanden inlezen of tekst printen op manieren die op de meeste systemen hetzelfde werken. Onder water is hier tussen bijvoorbeeld Linux en Windows echter heel andere code voor nodig. De standaard libraries bevatten algemene functies, die dit op het niveau van het besturingssysteem voor ons doen; wij gaan kijken hoe we Linux direct kunnen vragen om bepaalde functionaliteit.

We gaan een simpele shell maken, die op een aantal commando's kan reageren. Hiervoor maken we zoveel mogelijk gebruik van de Linux system calls. Je kunt het bestand `shell.cc` uit [deze repository](https://github.com/HU-TI-V1EOS/V1EOS-practica) gebruiken als uitgangspunt. 

De shell moet bij opstarten een configuratiebestand inlezen, waarin een enkele string staat. Deze string bevat het prompt (iedere invoerregel begint met het prompt om aan te geven dat de gebruiker invoer kan geven). Dit bestand wordt ingelezen met behulp van `read`. Als er in je configuratiebestand bijvoorbeeld `Geef een commando: ` staat dan moet die tekst iedere keer dat er een nieuw commando gevraagd wordt getoond worden.

Daarnaast moet de shell tenminste de volgende commando's ondersteunen:

- `new_file`: deze vraagt de gebruiker om twee strings: eerst een enkele regel, de bestandsnaam, en daarna een tekst, die afgesloten wordt met `<EOF>`. De gegeven tekst wordt in een nieuw bestand geschreven met de gekozen bestandsnaam. Gebruik hiervoor de system calls `creat`, `open` en `write`.
- `ls`: deze spawnt een nieuw proces (`ls -la`) en print het resultaat. Gebruik hiervoor de system calls `fork`, `exec` en `wait`.
- `find`: deze vraagt om een string, en spawnt vervolgens twee nieuwe processen, (`find .`) en (`grep string`). De processen worden met een pipe verbonden zodat `grep` de output van `find` filtert alvorens deze te printen. Gebruik hiervoor de system calls `fork`, `exec`, `wait`, `dup2`, `close` en `pipe`.
- `seekTest`: maakt twee bestanden, `seek` en `loop`. In beide bestanden wordt een `x` geschreven, gevolgd door 5 megabyte aan lege ruimte, gevolgd door nog een `x`. In het bestand `seek` gebeurt dit door gebruik te maken van de system-call `LSEEK`, waarmee de offset van de file descriptor van een open bestand naar voren wordt geplaatst. Voor het bestand `loop` wordt ~5 miljoen keer een "\0" karakter geschreven met een `for` loop. Vergelijk de snelheid van beide bestanden, en kijk met `ls -lh` (leesbare file size) en `ls -lS` (file size in daadwerkelijke blokken) naar beide bestanden. Als het goed is hebben beide bestanden volgens `ls -lh` hetzelfde formaat, maar niet volgens `ls -lS`.

**Let op: het is bij deze opdracht *verplicht* om gebruik te maken van plain system calls, zoals op de slides en in de voorbeeld-functie `src` te zien is. Het is absoluut niet toegestaan de functie `system()` te gebruiken (deze heeft niets met ruwe system-calls te maken), omdat je dan een shell aanroept terwijl het doel is zelf een shell te programmeren. Ga ervan uit dat je de genoemde system-calls bij de functies nodig zal hebben, als je er een mist heb je vermoedelijk iets niet goed gedaan!**
