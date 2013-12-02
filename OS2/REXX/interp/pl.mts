#
# pl.mts Version 1, 22-08-2001
#
# Regina error messages - Polish
# Original in English Written by Mark Hessling <M.Hessling@qut.edu.au>
# Translated by Bartosz Kozlowski <bk@bn.pl>
#
#
# NOTES (Bartosz Kozlowski):
# -----
# 1. Codepage IBM-CP852 (Latin 2) was selected for this translation.
# 2. Some messages required verbal and grammatical tricks that caused weird, though acceptable, results.
# 3. Here and there, there are some inconsistencies (deliberate or not) in translations of particular expressions.
# 4. This translation was not thoroughly tested so sometimes there may come out some buggy messages out of context.
# 5. Please report "weird" messages and all suggestions to me <bk@bn.pl>.
# 6. Current version of this translation is always available from Regina REXX Translation Project at http://www.bn.pl/~bk/serv/rrtp/
#
#
# UWAGI (Bartosz Kozlowski):
# -----
# 1. Do tego t�umaczenia zosta�a wybrana strona kodowa IBM-CP852 (Latin 2).
# 2. Niekt�re komunikaty wymaga�y ekwilibrystyki s�ownej i gramatycznej daj�c w efekcie dziwne, cho� dopuszczalne, rezultaty.
# 3. Gdzieniegdzie wyst�puj� niekonsekwencje (celowe lub nie) w t�umaczeniu niekt�rych zwrot�w.
# 4. To t�umaczenie nie zosta�o gruntownie przetestowane, wobec czego czasem mog� wychodzi� krzaki z kontekstu.
# 5. Prosz� o zg�aszanie do mnie <bk@bn.pl> "dziwnych" komunikat�w oraz wszelkich sugestii.
# 6. Aktualna wersja tego t�umaczenia jest zawsze dost�pna ze strony Projektu T�umaczenia Regina REXX pod adresem http://www.bn.pl/~bk/serv/rrtp/
#
#
#
#
#
  0,  1,B��d %s podczas dzia�ania %s, linia %d:|<value>,<source>,<linenumber>
  0,  2,B��d %s w interaktywnym �ladzie:|<value>
  0,  3,Interaktywny �lad.  "Trace Off", aby zako�czy� "debugowanie". ENTER, by kontynuowa�.
  2,  0,B��d podczas finalizacji
  2,  1,B��d podczas finalizacji: %s|<description>
  3,  0,B��d podczas inicjalizacji
  3,  1,B��d podczas inicjalizacji: %s|<description>
  4,  0,Program zosta� przerwany
  4,  1,Program przerwany z warunkiem stop'u (HALT): %s|<description>
  5,  0,Zasoby systemowe zosta�y wyczerpane
  5,  1,Zasoby systemowe zosta�y wyczerpane: %s|<description>
  6,  0,Niedomkni�ty "/*", apostrof lub cudzys��w
  6,  1,Niedomkni�ty separator komentarza ("/*")
  6,  2,Niedomkni�ty apostrof (')
  6,  3,Niedomkni�ty cudzys��w (")
  7,  0,Spodziewane WHEN lub OTHERWISE
  7,  1,SELECT w linii %d wymaga WHEN; znaleziono "%s"|<linenumber>,<token>
  7,  2,SELECT w linii %d wymaga WHEN, OTHERWISE, lub END; znaleziono "%s"|<linenumber>,<token>
  7,  3,W SELECT z linii %d wszystkie wyra�enia WHEN s� fa�szywe; spodziewane OTHERWISE|<linenumber>
  8,  0,Niespodziewane THEN lub ELSE
  8,  1,THEN nie ma odpowiadaj�cej klauzuli IF lub WHEN
  8,  2,ELSE nie ma odpowiadaj�cej klauzuli THEN
  9,  0,Niespodziewane WHEN lub OTHERWISE
  9,  1,WHEN nie ma odpowiadaj�cego SELECT
  9,  2,OTHERWISE nie ma odpowiadaj�cego SELECT
 10,  0,Niespodziewane lub niesparowane END
 10,  1,END nie ma odpowiadaj�cego DO lub SELECT
 10,  2,Po END odpowiadaj�cemu DO z linii %d musi wyst�pi� symbol, kt�ry odpowiada zmiennej steruj�cej (lub brak symbolu); znaleziono "%s"|<linenumber>,<token>
 10,  3,Po END odpowiadaj�cemu DO z linii %d nie mo�e wyst�pi� symbol, poniewa� nie ma zmiennej steruj�cej; znaleziono "%s"|<linenumber>,<token>
 10,  4,Po END odpowiadaj�cemu SELECT z linii %d nie mo�e wyst�pi� symbol; znaleziono "%s"|<linenumber>,<token>
 10,  5,END nie mo�e wyst�pi� bezpo�rednio po THEN
 10,  6,END nie mo�e wyst�pi� bezpo�rednio po ELSE
 11,  0,[Stos steruj�cy pe�ny]
 12,  0,[Klauzula > 1024 znak�w]
 13,  0,Niedozwolony znak w programie
 13,  1,Niedozwolony znak w programie "('%x'X)"|<hex-encoding>
 14,  0,Niekompletne DO/SELECT/IF
 14,  1,Instrukcja DO wymaga zamykaj�cego END
 14,  2,Instrukcja SELECT wymaga zamykaj�cego END
 14,  3,Po THEN wymagana jest instrukcja
 14,  4,Po ELSE wymagana jest instrukcja
 15,  0,Niepoprawny �a�cuch szesnastkowy lub dw�jkowy
 15,  1,Niepoprawne miejsce bia�ego znaku na pozycji %d w �a�cuchu szesnastkowym|<position>
 15,  2,Niepoprawne miejsce bia�ego znaku na pozycji %d w �a�cuchu dw�jkowym|<position>
 15,  3,Jedynie 0-9, a-f, A-F, oraz bia�e znaki s� dopuszczalne w �a�cuchu szesnastkowym; znaleziono "%c"|<char>
 15,  4,Jedynie 0, 1, oraz bia�e znaki s� dopuszczalne w �ancuchu dw�jkowym; znaleziono "%c"|<char>
 16,  0,Nie znaleziono etykiety
 16,  1,Etykieta "%s" nie zosta�a znaleziona|<name>
 16,  2,Nie mo�na u�y� SIGNAL do etykiety "%s", poniewa� znajduje si� wewn�trz grupy IF, SELECT lub DO |<name>
 16,  3,Nie mo�na wywo�a� etykiety "%s", poniewa� znajduje si� wewn�trz grupy IF, SELECT lub DO|<name>
 17,  0,Niespodziewane PROCEDURE
 17,  1,PROCEDURE jest dopuszczalne tylko je�li jest pierwsz� instrukcj� wykonywan� po wewn�trznym CALL lub wywo�aniu funkcji
 18,  0,Spodziewane THEN
 18,  1,S�owo kluczowe IF w linii %d wymaga odpowiadaj�cej klauzuli THEN; znaleziono "%s"|<linenumber>,<token>
 18,  2,S�owo kluczowe WHEN w linii %d wymaga odpowiadaj�cej klauzuli THEN; znaleziono "%s"|<linenumber>,<token>
 19,  0,Spodziewany �a�cuch albo symbol
 19,  1,Spodziewany �a�cuch albo symbol po s�owie kluczowym ADDRESS; znaleziono "%s"|<token>
 19,  2,Spodziewany �a�cuch albo symbol po s�owie kluczowym CALL; znaleziono "%s"|<token>
 19,  3,Spodziewany �a�cuch albo symbol po s�owie kluczowym NAME; znaleziono "%s"|<token>
 19,  4,Spodziewany �a�cuch albo symbol po s�owie kluczowym SIGNAL; znaleziono "%s"|<token>
 19,  6,Spodziewany �a�cuch albo symbol po s�owie kluczowym TRACE; znaleziono "%s"|<token>
 19,  7,Oczekiwano symbolu we wzorcu rozbioru gramatycznego; znaleziono "%s"|<token>
 20,  0,Spodziewana nazwa
 20,  1,Wymagana nazwa; znaleziono "%s"|<token>
 20,  2,Znaleziono "%s" tam, gdzie tylko nazwa jest mo�e by� u�yta|<token>
 21,  0,Niepoprawne dane na ko�cu klauzuli
 21,  1,Klauzula zako�czona niespodziewanym znacznikiem; znaleziono "%s"|<token>
 22,  0,Niepoprawny �a�cuch znakowy
 22,  1,Niepoprawny �a�cuch znakowy '%s'X|<hex-encoding>
 23,  0,Niepoprawny �a�cuch danych
 23,  1,Niepoprawny �a�cuch danych '%s'X|<hex-encoding>
 24,  0,Niepoprawne ��danie TRACE
 24,  1,Litera ��dania TRACE musi by� jedn� z "%s"; znaleziono "%c"|ACEFILNOR,<value>
 25,  0,Niepoprawne pod-s�owo kluczowe
 25,  1,Po CALL ON musi wyst�powa� jedno ze s��w kluczowych %s; znaleziono "%s"|<keywords>,<token>
 25,  2,Po CALL OFF musi wyst�powa� jedno ze s��w kluczowych %s; znaleziono "%s"|<keywords>,<token>
 25,  3,Po SIGNAL ON musi wyst�powa� jedno ze s��w kluczowych %s; znaleziono "%s"|<keywords>,<token>
 25,  4,Po SIGNAL OFF musi wyst�powa� jedno ze s��w kluczowych %s; znaleziono "%s"|<keywords>,<token>
 25,  5,Po ADDRESS WITH musi wyst�powa� jedno ze s��w kluczowych INPUT, OUTPUT lub ERROR; znaleziono "%s"|<token>
 25,  6,Po INPUT musi wyst�powa� jedno ze s��w kluczowych STREAM, STEM, LIFO, FIFO lub NORMAL; znaleziono "%s"|<token>
 25,  7,Po OUTPUT musi wyst�powa� jedno ze s��w kluczowych STREAM, STEM, LIFO, FIFO, APPEND, REPLACE lub NORMAL; znaleziono "%s"|<token>
 25,  8,Po APPEND musi wyst�powa� jedno ze s��w kluczowych STREAM, STEM, LIFO lub FIFO; znaleziono "%s"|<token>
 25,  9,Po REPLACE musi wyst�powa� jedno ze s��w kluczowych STREAM, STEM, LIFO lub FIFO; znaleziono "%s"|<token>
 25, 11,Po NUMERIC FORM musi wyst�powa� jedno ze s��w kluczowych %s; znaleziono "%s"|<keywords>,<token>
 25, 12,Po PARSE musi wyst�powa� jedno ze s��w kluczowych %s; znaleziono "%s"|<keywords>,<token>
 25, 13,Po UPPER musi wyst�powa� jedno ze s��w kluczowych %s; znaleziono "%s"|<keywords>,<token>
 25, 14,Po ERROR musi wyst�powa� jedno ze s��w kluczowych STREAM, STEM, LIFO, FIFO, APPEND, REPLACE lub NORMAL; znaleziono "%s"|<token>
 25, 15,Po NUMERIC musi wyst�powa� jedno ze s��w kluczowych %s; znaleziono "%s"|<keywords>,<token>
 25, 16,Po FOREVER musi wyst�powa� jedno ze s��w kluczowych %s; znaleziono "%s"|<keywords>,<token>
 25, 17,Po PROCEDURE musi wyst�powa� s�owo kluczowe EXPOSE lub nie powinno wyst�powa� nic; znaleziono "%s"|<token>
 26,  0,Niepoprawna liczba ca�kowita
 26,  1,Liczby ca�kowite musz� by� dostosowane do obecnego ustawienia DIGITS(%d); znaleziono "%s"|<value>,<value>
 26,  2,Warto�� wyra�eni liczby powt�rze� w instrukcji DO musi by� liczb� ca�kowit� nieujemn�; znaleziono "%s"|<value>
#
 26,  3,Warto�� wyra�enia FOR w instrukcji DO musi by� liczb� ca�kowit� nieujemn�; znaleziono "%s"|<value>
 26,  4,Parametr pozycyjny szablonu rozbioru gramatycznego musi by� liczb� ca�kowit�; znaleziono "%s"|<value>
 26,  5,Warto�� NUMERIC DIGITS  musi by� liczb� ca�kowit� nieujemn�; znaleziono "%s"|<value>
 26,  6,Wato�� NUMERIC FUZZ musi by� liczb� ca�kowit� nieujemn�; znaleziono "%s"|<value>
 26,  7,Liczba u�yta w ustawieniu TRACE musi by� ca�kowita; znaleziono "%s"|<value>
 26,  8,Wyk�adnik operatora pot�gowania ("**") musi by� liczb� ca�kowit�; znaleziono "%s"|<value>
 26, 11,Rezultat operacji %s %% %s wymaga�by wyk�adniczej notacji przy obecnym NUMERIC DIGITS %d|<value>,<value>,<value>
 26, 12,Rezultat operacji %% u�yty dla operacji %s // %s wymaga�by wyk��dniczej notacji przy obecnym NUMERIC DIGITS %d|<value>,<value>,<value>
 27,  0,Niepoprawna sk�adnia DO
 27,  1,Niepoprawne u�ycie s�owa kluczowego "%s" w klauzuli DO|<token>
 28,  0,Niepoprawne LEAVE lub ITERATE
 28,  1,LEAVE jest dopuszczalne tylko w obszarze powtarzaj�cej p�tli DO
 28,  2,ITERATE jest dopuszczalne tylko w obszarze powtarzaj�cej p�tli DO
 28,  3,Symbol nast�puj�cy po LEAVE ("%s") musi odpowiada� b�d� zmiennej steruj�cej, b�d� musi zosta� pomini�ty|<token>
 28,  4,Symbol nast�puj�cy po ITERATE ("%s") musi odpowiada� b�d� zmiennej steruj�cej, b�d� musi zosta� pomini�ty|<token>
 29,  0,Nazwa �rodowiska jest za d�uga
 29,  1,D�ugo�� nazwy �rodowiska przekracza %d znak(i/�w); znaleziono "%s"|#Limit_EnvironmentName,<name>
 30,  0,Za d�uga nazwa lub �a�cuch
 30,  1,D�ugo�� nazwy przekracza %d znak(i/�w)|#Limit_Name
 30,  2,Litera� �a�cuchowy przekracza d�ugo�� %d znak(i/ow)|#Limit_Literal
 31,  0,Nazwa zaczyna si� cyfr� lub "."
 31,  1,Nie mo�na przypisa� warto�ci liczbie; znaleziono "%s"|<token>
 31,  2,Nazwa zmiennej nie mo�e zaczyna� si� cyfr�; znaleziono "%s"|<token>
 31,  3,Nazwa zmiennej nie mo�e zaczyna� si� "."; znaleziono "%s"|<token>
 32,  0,[Niepoprawne u�ycie stem]
 33,  0,Niepoprawny wynik wyra�enia
 33,  1,Warto�� NUMERIC DIGITS "%d" musi przekracza� warto�� NUMERIC FUZZ "(%d)"|<value>,<value>
 33,  2,Warto�� NUMERIC DIGITS "%d" nie mo�e przekracza� %d|<value>,#Limit_Digits
 33,  3,Wynik wyra�enia nast�puj�cego po NUMERIC FORM musi zaczyna� si� od "E" lub "S"; znaleziono "%s"|<value>
 34,  0,Warto�� logiczna nie jest "0" lub "1"
 34,  1,Warto�� wyra�enia nast�puj�cego po s�owie kluczowym IF musi by� dok�adnie "0" lub "1"; znaleziono "%s"|<value>
 34,  2,Warto�� wyra�enia nast�puj�cego po s�owie kluczowym WHEN musi by� dok�adnie "0" lub "1"; znaleziono "%s"|<value>
 34,  3,Warto�� wyra�enia nast�puj�cego po s�owie kluczowym WHILE musi by� dok�adnie "0" lub "1"; znaleziono "%s"|<value>
 34,  4,Warto�� wyra�enia nast�puj�cego po s�owie kluczowym UNTIL musi by� dok�adnie "0" lub "1"; znaleziono "%s"|<value>
 34,  5,Warto�� wyra�enia po lewej operatora logicznego "%s" musi by� dok�adnie "0" lub "1"; znaleziono "%s"|<operator>,<value>
 34,  6,Warto�� wyra�enia po prawej operatora logicznego "%s" musi by� dok�adnie "0" lub "1"; znaleziono "%s"|<operator>,<value>
 35,  0,Niepoprawne wyra�enie
 35,  1,Wykryto niepoprawne wyra�enie w "%s"|<token>
 36,  0,Niedomkni�ty "(" w wyra�eniu
 37,  0,Niespodziewany "," or ")"
 37,  1,Niespodziewany ","
 37,  2,Nieotwarty ")" w wyra�eniu
 38,  0,Niepoprawny szablon lub wz�r
 38,  1,Wykryto niepoprawny szablon rozbioru gramatycznego w "%s"|<token>
 38,  2,Wykryto niepoprawn� pozycj� rozbioru gramatycznego w "%s"|<token>
 38,  3,Instrukcja PARSE VALUE wymaga s�owa kluczowego WITH
 39,  0,[Przepe�nienie stosu warto�ciowania]
 40,  0,Niepoprawne wywo�anie podprogramu
 40,  1,Zawi�d� zewn�trzny podprogram "%s"|<name>
 40,  3,Niewystarczaj�ca liczba argument�w przy wywo�aniu "%s"; oczekiwane minimum to %d|<bif>,<argnumber>
 40,  4,Zbyt du�a liczba argument�w przy wywo�aniu "%s"; oczekiwane maksimum to %d|<bif>,<argnumber>
 40,  5,Brakuj�cy argument przy wyo�aniu "%s"; argument %d jest wymagany|<bif>,<argnumber>
 40,  9,W %s argument %d wyk�adnik przekracza %d cyfr(y); znaleziono "%s"|<bif>,<argnumber>,#Limit_ExponentDigits,<value>
 40, 11,W %s argument %d musi by� liczb�; znaleziono "%s"|<bif>,<argnumber>,<value>
 40, 12,W %s argument %d musi by� liczb� ca�kowit�; znaleziono "%s"|<bif>,<argnumber>,<value>
 40, 13,W %s argument %d musi by� liczb� nieujemn�; znaleziono "%s"|<bif>,<argnumber>,<value>
 40, 14,W %s argument %d musi by� dodatni; znaleziono "%s"|<bif>,<argnumber>,<value>
 40, 17,Argument %s nr 1, musi mie� cz��� ca�kowit� z zakresu 0:90 i cz��� dziesi�tn� nie wi�ksz� ni� .9; znaleziono "%s"|<bif>,<value>
 40, 18,Rok w konwersji %s musi by� z zakresu od 0001 to 9999|<bif>
 40, 19,Argument %s nr 2, "%s", nie jest w formacie opisanym przez 3 argument, "%s"|<bif>,<value>,<value>
 40, 21,W %s argument %d nie mo�e by� pusty|<bif>,<argnumber>
 40, 23,W %s argument %d musi by� pojedynczym znakiem; znaleziono "%s"|<bif>,<argnumber>,<value>
 40, 24,W %s argument 1 musi by� �a�cuchem dw�jkowym; znaleziono "%s"|<bif>,<value>
 40, 25,W %s argument 1 musi by� �a�cuchem szesnastkowym; znaleziono "%s"|<bif>,<value>
 40, 26,Argument %s nr 1 musi by� a valid symbol; found "%s"|<bif>,<value>
 40, 27,Argument %s nr 1, musi by� poprawn� nazw� strumienia; znaleziono "%s"|<bif>,<value>
 40, 28,Argument %s nr %d, opcja musi zaczyna� si� jednym z "%s"; znaleziono "%s"|<bif>,<argnumber>,<optionslist>,<value>
 40, 29,Konwersja %s do formatu "%s" nie jest dozwolona|<bif>,<value>
 40, 31,Argument %s nr 1 ("%d") nie mo�e przekracza� 100000|<bif>,<value>
 40, 32,W %s r��nica mi�dzy argumentem 1 ("%d") a argumentem 2 ("%d") nie mo�e przekracza� 100000|<bif>,<value>,<value>
 40, 33,Argument %s nr 1 ("%d") musi by� mniejszy lub r�wny od argumentu 2 ("%d")|<bif>,<value>,<value>
 40, 34,Argument %s nr 1 ("%d") musi by� mniejszy lub r�wny od liczby linii w programie (%d)|<bif>,<value>,<sourceline()>
 40, 35,Argument %s nr 1 nie mo�e by� wyra�ony jako liczba ca�kowita; znaleziono "%s"|<bif>,<value>
 40, 36,W %s argument 1 musi by� nazw� zmiennej zasobu; znaleziono "%s"|<bif>,<value>
 40, 37,W %s argument 3 musi by� nazw� zasobu; znaleziono "%s"|<bif>,<value>
 40, 38,Argument %s nr %d nie jest wystarczaj�co du�y aby sformatowa� "%s"|<bif>,<argnumber>,<value>
 40, 39,Argument %s nr 3 nie jest zerem lub jedynk�; znaleziono "%s"|<bif>,<value>
 40, 41,W %s argument %d musi zawiera� si� w ograniczeniach strumienia; znaleziono "%s"|<bif>,<argnumber>,<value>
 40, 42,Argument %s nr 1; nie powinien wyst�pi� w strumieniu; znaleziono "%s"|<bif>,<value>
 40,914,[Argument %s nr %d, musi by� jednym z "%s"; znaleziono "%s"]|<bif>,<argnumber>,<optionslist>,<value>
 40,920,[%s: niskopoziomowy b��d I/O strumienia; %s]|<bif>,<description>
 40,921,[Argument %s nr %d, tryb pozycjonowania strumienia "%s"; niekompatybilny z trybem otwarcia strumienia]|<bif>,<argnumber>,<value>
 40,922,[Argument %s nr %d, za ma�o pod-polece�; oczekiwane minimum to %d; znaleziono %d]|<bif>,<argnumber>,<value>,<value>
 40,923,[Argument %s nr %d, za wiele pod-polece�; oczekiwane maksimum to %d; znaleziono %d]|<bif>,<argnumber>,<value>,<value>
 40,924,[Argument %s nr %d, niepoprawny parametr pozycjonowania; spodziewane jedno z "%s"; znaleziono "%s"]|<bif>,<argnumber>,<value>,<value>
 40,930,[RXQUEUE, funkcja TIMEOUT, spodziewano liczby ca�kowitej z zakresu od -1 do %d; znaleziono \"%s\"]|<value>,<value>
 40,980,Niespodziewana warto�� na wej�ciu, albo nieznany typ albo niedozwolone dane%s%s|:,<location>
 40,981,Liczba poza dozwolonym zakresem%s%s|:,<location>
 40,982,�a�cuch za du�y dla zdefiniowanego bufora%s%s|:,<location>
 40,983,Niepoprawna kombinacja typ/rozmiar%s%s|:,<location>
 40,984,Niewspierana liczba jak NAN, +INF, -INF%s%s|:,<location>
 40,985,Struktura zbyt z�o�ona dla statycznego, wewn�trznego bufora%s%s|:,<location>
 40,986,Brak elementu struktury%s%s|:,<location>
 40,987,Brak warto�ci struktury%s%s|:,<location>
 40,988,Nazwa b�d� jej cz��� jest niedozwolona dla interpretera%s%s|:,<location>
 40,989,Wyst�pi� problem na interfejsie mi�dzy Regin� a GCI%s%s|:,<location>
 40,990,Typ nie spe�nia wymaga� dla typ�w podstawowych (argumenty/zwracana warto��)%s%s|:,<location>
 40,991,Niepoprawna liczba argument�w%s%s|:,<location>
 40,992,GCI's internal stack for arguments got an overflow%s%s|: ,<location>
 40,993,GCI counted too many nested LIKE containers%s%s|: ,<location>
 41,  0,Niepoprawna konwersja arytmetyczna
 41,  1,Nieliczbowa warto�� ("%s") lewej strony operacji arytmetycznej "%s"|<value>,<operator>
 41,  2,Nieliczbowa warto�� ("%s") prawej strony operacji arytmetycznej "%s"|<value>,<operator>
 41,  3,Nieliczbowa warto�� ("%s") u�yta z przedrostkiem opertora "%s"|<value>,<operator>
 41,  4,Warto�� wyra�enia TO w instrukcji DO musi by� liczbow�; znaleziono "%s"|<value>
 41,  5,Warto�� wyra�enia BY w instrukcji DO musi by� liczbow�; znaleziono "%s"|<value>
 41,  6,Warto�� wyra�enia zmiennej steruj�cej w instrucji DO musi by� liczbow�; znaleziono "%s"|<value>
 41,  7,Wyk�adnik przekracza %d cyfr; znaleziono "%s"|#Limit_ExponentDigits,<value>
 42,  0,Nadmiar/niedomiar arytmetyczny
 42,  1,Wykryto nadmiar arytmetyczny w "%s %s %s"; wyraziciel rezultatu wymaga wi�cej ni� %d cyfr|<value>,<operator>,<value>,#Limit_ExponentDigits
 42,  2,Wykryto niedomiar arytmetyczny w "%s %s %s"; wyraziciel rezultatu wymaga wi�cej ni� %d cyfr|<value>,<operator>,<value>,#Limit_ExponentDigits
 42,  3,Nadmiar arytmetyczny; dzielnik nie mo�e by� zerem
 43,  0,Nie znaleziono podprogramu
 43,  1,Nie mo�na odnale�� podprogramu "%s"|<name>
 44,  0,Funkcja nie zwr�ci�a danych
 44,  1,Brak danych zwr�conych przez funkcj� "%s"|<name>
 45,  0,Brak danych wyspecyfikowanych przy RETURN funkcji
 45,  1,Spodziewano danych z instrukcj� RETURN, poniewa� podprogram "%s" zosta� wywo�any jako funkcja|<name>
 46,  0,Niepoprawne odniesienie do zmiennej
 46,  1,Znaleziono dodatkowy sk�adnik ("%s") w odniesieniu zmiennej; ")" spodziewano|<token>
 47,  0,Niespodziewana etykieta
 47,  1,Dane INTERPRET nie mog� zawiera� etytkiet; znaleziono "%s"|<name>
 48,  0,B��d w us�udze systemowej
 48,  1,B��d w us�udze systemowej: %s|<description>
 48,920,Niskopoziomowy b��d strumienia I/O: %s %s: %s|<description>,<stream>,<description>
 49,  0,B��d interpretacji
 49,  1,B��d interpretacji: B��d w %s, linia %d: "%s". Prosz� zg�osi� ten b��d!|<module>,<linenumber>,<description>
 50,  0,Nierozpoznany zastrze�ony symbol
 50,  1,Nierozpoznany zastrze�ony symbol "%s"|<token>
 51,  0,Niepoprawna nazwa funkcji
 51,  1,Nazwy funkcji nie uj�te w cudzys��w nie mog� ko�czy� si� kropk�; znaleziono "%s"|<token>
 52,  0,Wynik zwr�cony przez "%s" jest d�u�szy ni� %d znak(i/�w)|<name>,#Limit_String
 53,  0,Niepoprawna opcja
 53,  1,�a�cuch lub symbol oczekiwany po s�owie kluczowym STREAM; znaleziono "%s"|<token>
 53,  2,Po s�owie kluczowym STEM oczekiwane odniesienie do zmiennej; znaleziono "%s"|<token>
 53,  3,Argument do STEM musi mie� jedn� kropk� jako ostatni znak; znaleziono "%s"|<name>
 53,100, �a�cuch lub symbol spodziewane po s�owie kluczowym LIFO; znaleziono "%s"|<token>
 53,101,�a�cuch lub symbol spodziewane po s�owie kluczowym FIFO; znaleziono "%s"|<token>
 54,  0,Niepoprawna warto�� STEM
 54,  1,Dla tego STEM APPEND, warto�� "%s" musi by� liczb� linii; znaleziono "%s"|<name>,<value>
#
# Wszystkie poni�sze komunikaty nie s� zdefiniowane przez ANSI
#
 60,  0,[Nie mo�na cofn�� kursora pliku tymczasowego]
 61,  0,[Niepoprawna operacja pozycjonowania na pliku]
 64,  0,[B��d sk�adni podczas rozbioru gramatycznego]
 64,  1,[B��d sk�adni w linii %d]
 64,  2,[Podstawowy b��d sk��dni w linii %d, kolumna %d]|<linenumber>,<columnnumber>
 90,  0,[Opcja niezgodna z ANSI u�yta przy "OPTIONS STRICT_ANSI"]
 90,  1,[%s jest funkcj� BIF rozszerzenia Regina'y]|<bif>
 90,  2,[%s jest instrukcj� rozszerzenia Regina'y]|<token>
 90,  3,[Argument %s nr %d, Przy "OPTIONS STRICT_ANSI" opcja musi zaczyna� si� jednym z "%s; znaleziono "%s"; rozszerzenie Regina'y]|<bif>,<argnumber>,<optionslist>,<value>
 93,  0,[Niepoprawne wywo�anie procedury]
 93,  1,[Po komendzie STREAM %s musi wyst�powa� jedna z "%s"; znaleziono "%s"]|<token>,<value>,<value>
 93,  3,[Komenda STREAM musi by� jedn� z "%s"; znaleziono "%s"]|<value>,<value>
 94,  0,[B��d interfejsu zewn�trznej kolejki]
 94, 99,[Wewn�trzny b��d z interfejsem zewn�trznej kolejki: %d "%s"]|<description>,<systemerror
 94,100,[Podstawowy b��d systemu z interfejsem zewn�trznej kolejki. %s. %s]|<description>,<systemerror>
 94,101,[B��d podczas ��czenia si� z %s na porcie %d: "%s"]|<machine>,<portnumber>,<systemerror>
 94,102,[Nie mo�na uzyska� adresu IP dla %s]|<machine>
 94,103,[Niepoprawny format dla serwera w podanej nazwie kolejki: "%s"]|<queuename>
 94,104,[Niepoprawny format nazwy kolejki: "%s"]|<queuename>
 94,105,[Nie mo�na uruchomi� interfejsu gniazd systemu Windows: %s]|<systemerror>
 94,106,[Przekroczono maksymaln� liczb� zewn�trznych kolejek: %d]|<maxqueues>
 94,107,[Wyst�pi� b��d podczas czytania z gniazda: %s]|<systemerror>
 94,108,[Przekazano niepoprawny prze��cznik. Musi by� jednym z "%s"]|<switch>
 94,109,[Kolejka \"%s\" nie znaleziona]|<queuename>
 94,110,[%s niepoprawna dla zewn�trznych kolejek]|<bif>
 94,111,[Funkcja RXQUEUE %s niepoprawna dla wewn�trznych kolejek]|<functionname>
 95,  0,[Zastrze�ona opcja u�yta w trybie "bezpiecznym"]
 95,  1,[%s niedozwolony w trybie "bezpiecznym"]|<token>
 95,  2,[%s argument %d niedozwolony w trybie "bezpiecznym"]|<bif>,<argnumber>
 95,  3,[Argument %s nr %d: "%s", niedozwolony w trybie "bezpiecznym"]|<bif>,<argnumber>,<token>
 95,  4,[3 argument STREAM: Otwieranie plik�w w trybie WRITE jest niepoprawne w trybie "bezpiecznym"]
 95,  5,[Uruchamianie polece� zewn�trznych jest niedozwolone w trybie "bezpiecznym"]
100,  0,[Nieznany b��d systemu plik�w]
#
# Extra general phrases requiring translation:
# The text in () is the corresponding array name in error.c
#
# (err1prefix)   "B��d %d podczas dzia�ania \"%.*s\", linia %d: %.*s",
# (suberrprefix) "B��d %d.%d: %.*s",
# (err2prefix)   "B��d %d podczas dzia�ania \"%.*s\": %.*s",
# (erropen)      "Nie mo�na otworzy� pliku j�zyka: %s",
# (errcount)     "Niepoprawna liczba komunikat�w w pliku j�zyka: %s",
# (errread)      "Nie mo�na czyta� z pliku j�zyka: %s",
# (errmissing)   "Brakuje tekstu w pliku j�zyka: %s.mtb",
# (errcorrupt)   "Plik j�zyka: %s.mtb jest zniekszta�cony",
