---
title: "EOS Practicum — Shell Scripting"
keywords: [EOS]
---

# EOS Practicum — Shell Scripting

Deze opdracht is individueel. De opdracht is het maken van een aantal Shell scripts, die je in moet leveren via GitHub.

We hebben inmiddels gezien dat Linux met een grote lading simpele command-line programma's komt, die met pipes te combineren zijn. De shell kan echter nog veel meer: in principe is het een full-fledged programmeertaal met condities (`if` en `switch`), loops en subroutines. Door een serie commando's en statements in een bestand te zetten (een shellscript) kan je simpele taken automatiseren. In deze opdracht leggen we de basis van het programmeren in shell.

## Shellscripts
Een shellscript is in principe niets anders dan een tekstbestand met opdrachten voor de shell, vergelijkbaar met een Python script: de code hoeft niet (zoals bij C++) gecompileerd te worden, maar wordt direct door een interpreter uitgevoerd. In het geval van shellscript is de interpreter de shell waar je ook direct commando's invoert (doorgaans Bash). Als eerste maken we en simpele Hello World: maak een nieuw tekstbestand, `hello` (shellscripts hebben doorgaans geen extensie, al wordt `.sh` soms gebruikt). Zet hierin de volgende inhoud:

    #!/usr/bin/env bash
    echo "Hello World!"

In plaats van `/usr/bin/env bash` zul je ook vaak `/bin/bash` of `/bin/sh` tegenkomen. De eerste twee doen in principe hetzelfde, al is `/usr/bin/env bash` iets meer portable: je vraagt het systeem naar het pad van Bash, in plaats van dat je de meest gebruikelijke locatie kiest. Bij `/bin/sh` wordt een ouder, primitievere shell gebruikt: er zijn meerdere shells waar in gescript wordt, ouder of nieuwer dan Bash; Bash is in principe de standaard keuze, Sh is vooral voor heel oude systemen zinvol.

Om dit script te kunnen draaien moet het nog wel uitvoerbaar gemaakt worden, gebruik hiervoor het volgende commando in een terminal in de juiste directory. De `$` moet je niet overnemen, deze geeft aan dat je het commando in je terminal typt (dit is een conventie die je vaker tegenkomt bij Linux). Regels zonder `$` zijn dan uitvoer (of, bij een tekstblok zonder `$`, de inhoud van een bestand).

    $ chmod +x hello

Hierna kun je het bestand uitvoeren:

    $ ./hello
    Hello World!

## Zelf aan de slag
Nu gaan we zelf aan de slag met het schrijven van shellscripts. Een aantal dingen om op te letten:

- Maak één script per opdracht.
- Voor het testen van de shellscripts is een mappenstructuur beschikbaar. Gebruik het script `reset` alles weer terug te zetten voor een volgende test. **Let op:** zet je scripts zelf niet in deze map, anders worden deze door reset **verwijderd**.
- Gebruik Bash. Als je een alternatieve shell gebruikt, voer dan eerst `bash` uit voordat je je script als commando gebruikt.
- Documenteer je script: net als in Python kun je `#` gebruiken voor commentaar.
- Deze opdrachten gaan over de stof van 2 lessen. Mocht je ergens niet uitkomen, dan ben je mogelijk vooruit aan het lopen en heb je kennis nodig die volgende les wordt behandeld.
- Voorkom [Useless Use of Cat](http://porkmail.org/era/unix/award.html).
- Voorkom hardcoded paden, geef de doelmap als argument mee aan je shellscripts.
- **Lever je werk in door een link naar GitHub op te geven. Link hierbij naar een commit, niet naar de repository als geheel!**

## Opdracht 0
Maak een shellscript `sorteerAfbeeldingen` dat een map "Afbeeldingen" aanmaakt, en alle JPEG en PNG bestanden hiernaar verplaatst. Het script wordt aangeroepen met `sorteerAfbeeldingen /pad/naar/EOS1_bestanden`. Doe dit zowel met een expliciete loop, als met behulp van `find -exec` of `find | xargs` (naar keuze).

## Opdracht 1
Maak een shellscript `voerUit` dat afhankelijk van de extensie van een meegegeven bestand de juiste interpreter aanroept. Als de gebruiker `voerUit bestand.py` aanroept wordt binnen het shellscript `python3 bestand.py` aangeroepen. Bij `voerUit bestand.sh` wordt `bash bestand.sh` uitgevoerd. Bij `voerUit bestand.cc` wordt de code geprint met `cat bestand.cc`. Bij een ander bestandstype wordt een foutmelding geprint. Gebruik voor deze opdracht een `case` statement.

## Opdracht 2
Installeer voor deze opdracht ImageMagick (`sudo apt install imagemagick`). Maak een shellscript `naarPNG` dat ieder JPEG bestand in de map converteert naar een PNG van maximaal 128x128 pixels. Gebruik de `man`-page om je hierbij te helpen. Het script wordt aangeroepen met `naarPNG /pad/naar/EOS1_bestanden`. HINT: het commando dat je nodig hebt heet `convert`.

## Opdracht 3
Maak een shellscript `checkStatus` dat een gegeven commando uit probeert te voeren op ieder bestand in een gegeven map. Per bestand wordt in een gegeven logbestand geschreven of het commando geslaagd is of niet. Het logbestand wordt iedere keer dat `checkStatus` gedraaid wordt opnieuw aangemaakt. Het script wordt aangeroepen met `checkStatus /pad/naar/bestanden commando logfile`. Gebruik hiervoor de return status en `if` / `test`. Een goede test is om je script aan te roepen met `cat`: als alles goed werkt geeft dit voor mappen een error, en voor alle bestanden een exit-code `0` (succes). Je kunt IO redirectie gebruiken om de output en/of errors van `cat` te verbergen als dit een hoop ellende print (`cat` print ook binaire bestanden, maar het resultaat daarvan is niet mooi).

## Opdracht 4
Maak een shellscript `maakLogin` dat:

- Om je naam vraagt, en als deze niet wordt gegeven de huidige gebruikersnaam als default pakt (via het commando `whoami`);
- een wachtwoord vraagt, en dit met een `while`-loop blijft herhalen tot tweemaal hetzelfde wachtwoord wordt ingevoerd dat uit minstens 8 karakters bestaat;
- een tekstbestand schrijft met de gebruikersnaam en een MD5-hash (`md5sum`) van het gekozen wachtwoord.
Het script wordt aangeroepen met `maakLogin passwordfile`.

## Opdracht 5
Bedenk zelf een toepassing en programmeer deze in een shellscript. Let erop dat je hard-coded paden vermijdt.
