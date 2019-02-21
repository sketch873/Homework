Andrei-Costin Martin
336CA

  Problema 1

  -- CITIRE --
  Citesc headerul; in cazul in care imaginea este grayscale
  atunci folosesc doar vectorul 'gP' din structura imaginii
  altfel pe 'rP', 'gP' si 'bP', fiind RGB.

  -- SCRIERE --
  Scriu headerul; in functie de header trebuie scrisi ori
  'rP', 'gP' si 'bP' ori doar 'gP'.

  -- RESIZE --
  In functie de tipul header-ului se folosesc pentru
  paralelizare o functie specifica('resizeGrayscale' sau 'resizeColor').
  In ambele situatii paralelizeaza dupa inaltimea noii imagini
  (cea cu o inaltime mai mica). Fiecare thread modifica linia.

  Cum noua inaltime este data de relatia 'inaltime_veche/resize_factor'
  atunci se realireaza un numar de 'inaltime_veche/resize_factor' submultimi
  independente => problema concurentei este rezolvata.

  Problema 2

  -- INITIALIZARE --
  Initializez matricea pixelilor cu maxVal (alb).

  -- SCRIERE --
  Scriu headerul si matricea pixelilor.

  -- RANDARE --
  Am pacurs matricea si am verificat cu functia 'getDistance' daca
  punctul este suficient de aproape de dreapta data de ecuatia din enuntul
  temei. Daca este suficient de aproape atunci valoarea pixelului devine
  0 (negru).

  Din punct de vedere al paralelizarii, fiecare thread lucreaza de la start
  la stop, pe inaltimea imaginii. In acest fel, fiecare linie este colorata
  independent de alte valori ale matricei si dependent de formula, care
  este constanta => problema concurentei este rezolvata.
