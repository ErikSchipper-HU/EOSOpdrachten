#!/usr/bin/env bash

echo "[2J[H## V1EOS Shell test script ##"
echo
echo "Dit script test je shell-implementatie op correcte werking en veelgemaakte fouten. Als je script zonder fouten draait kun je de code op GitHub zetten en een link inleveren via Canvas voor feedback en om af te tekenen."
echo "Roep dit script aan met als argument de executable van je shell, bijvoorbeeld \"./shell\". Het script gaat ervan uit dat je source \"shell.cc\" heet."

if ! [ -f "shell.cc" ] ; then
  echo
  echo "[31mFout: shell.cc niet gevonden"
  echo
  exit -1
fi

if [ -f "$1" ] ; then
  MYSHELL=$1
else
  echo
  echo "Geen argument geleverd of executable niet gevonden, ik probeer \"./shell\"."
  if [ -f "shell" ] ; then
    MYSHELL="./shell"
  else
    echo "[31mGeen executable gevonden. Geef deze als argument mee (bijvoorbeeld \"./shell\") of zorg dat je programma gecompileerd is."
    echo
    exit -1
  fi
fi

if grep -q "system(" shell.cc; then
  echo
  echo "[31mHet lijkt erop dat je de C++ functie system() hebt gebruikt; dit is voor deze opdracht niet toegestaan. Je moet gebruik maken van system calls, gebruik hiervoor de C++ functie syscall()."
  exit
fi

echo
echo "[0mTesting new_file ..."

STR="Kalani Paiʻea Wohi o Kaleikini Kealiʻikui Kamehameha o ʻIolani i Kaiwikapu kauʻi Ka Liholiho Kūnuiākea"

rm -f Kamehameha
$MYSHELL > /dev/null << EOF
new_file
Kamehameha
Kalani Paiʻea Wohi o Kaleikini Kealiʻikui Kamehameha o ʻIolani i Kaiwikapu kauʻi Ka Liholiho Kūnuiākea
<EOF>
EOF

if [ -f Kamehameha ] && [ "$STR" == "$(cat Kamehameha)" ] ; then
  echo "[32mnew_file werkt!"
else
  echo "[31mnew_file werkt nog niet!"
  exit -1
fi
rm -f Kamehameha

echo
echo "[0mTesting ls ..."
STR="$(LC_ALL=C ls -la | tail -n +2 | LC_COLLATE=C sort)"
RES=$(echo "ls" | LC_ALL=C $MYSHELL | tail -n +2 | head -n -1 | LC_COLLATE=C sort)

if [ "$(echo $RES)" == "$(echo $STR)" ] ; then
  echo "[32mls werkt!"
else
  echo "[31mls werkt nog niet!"
  exit -1
fi

echo
echo "[0mTesting find ..."
STR=$(find . | grep shell | tail -n +2)
RES=$($MYSHELL << EOF | tail -n +2 | head -n -1 
find
shell
EOF
)

if [ "$(echo $RES)" == "$(echo $STR)" ] ; then
  echo "[32mfind werkt!"
else
  echo "[31mfind werkt nog niet!"
  exit -1
fi

echo
echo "[0mTesting seek ..."
echo "seek" | $MYSHELL > /dev/null

if [ $(stat -c "%b" seek) -lt $(stat -c "%b" loop) ] && diff loop seek ; then
  echo "[32mseek werkt!"
else
  echo "[31mseek werkt nog niet!"
  exit -1
fi
rm -f seek loop

echo
echo "[36mAlles lijkt te werken! Commit en push je code naar GitHub, en lever de link in op Canvas!"
echo
