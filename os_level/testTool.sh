#!/usr/bin/env bash

echo "[2J[H## V1EOS Tool test script ##"
echo
echo "Dit script roept je tool aan in een pipe-line. Omdat de invulling van de tool vrij is, kan ik niet geautomatiseerd testen dat de uitvoer correct is. Je kunt hier wel zelf aan zien of de invoer goed door je programma doorgesluisd wordt. Geef het pad van je tool (bijvoorbeeld \"./tool\") en een argument mee bij het aanroepen van dit script."
echo

if [ $# -lt 2 ] ; then
  echo "[31mFout: geef het pad naar je programma en een argument mee!"
  echo
  exit -1
fi

($1 $2 | lolcat -a) <<- 'EOF'
Gallia est omnis divisa in partes tres, quarum unam incolunt Belgae, aliam Aquitani, tertiam qui ipsorum lingua Celtae, nostra Galli appellantur.
Hi omnes lingua, institutis, legibus inter se differunt. Gallos ab Aquitanis Garumna flumen, a Belgis Matrona et Sequana dividit.
EOF

echo "Als deze output er naar verwachting uitziet kun je je code committen op GitHub en inleveren op via Canvas."
