# Passsende Codepages/Fitting Codepages:
# Windows 1252 (Western style)
# ISO Latin 1 aka 8859-1
# ISO Latin 9 aka 8859-15            (stupid renumbering!)
#
# Regina Fehlermeldungen - Deutsch
# Geschrieben von Mark Hessling <M.Hessling@qut.edu.au>
#
# Deutsch - <florian@grosse-coosmann.de>
#
  0,  1,Fehler %s in %s, Zeile %d:|<value>,<source>,<linenumber>
  0,  2,Fehler %s in der interaktiven Debugging-Sitzung:|<value>
  0,  3,Interaktives Debugging.  "Trace Off" zum Beenden. ENTER zum Fortfahren.
  2,  0,Fehler w�hrend der Programmbereinigung
  2,  1,Fehler w�hrend der Programmbereinigung: %s|<description>
  3,  0,Fehler w�hrend der Initialisierung
  3,  1,Fehler w�hrend der Initialisierung: %s|<description>
  4,  0,Programm unterbrochen
  4,  1,Programm unterbrochen mit dem HALT-Zustand: %s|<description>
  5,  0,Systemresourcen ersch�pft
  5,  1,Systemresourcen ersch�pft: %s|<description>
  6,  0,Fehlendes Ende zu "/*" oder Anf�hrungszeichen
  6,  1,Fehlendes Kommentarende zu ("/*")
  6,  2,Fehlendes zweites Hochkomma (')
  6,  3,Fehlendes zweites Anf�hrungszeichen (")
  7,  0,WHEN oder OTHERWISE erwartet
  7,  1,SELECT in Zeile %d erwartet WHEN; "%s" gefunden|<linenumber>,<token>
  7,  2,SELECT in Zeile %d erwartet WHEN, OTHERWISE oder END; "%s" gefunden|<linenumber>,<token>
  7,  3,Alle WHEN-Ausdr�cke des SELECT in Zeile %d sind falsch; OTHERWISE erwartet|<linenumber>
  8,  0,Unerwartetes THEN oder ELSE
  8,  1,THEN hat keine zugeh�rige IF- oder WHEN-Klausel
  8,  2,ELSE hat keine zugeh�rige THEN-Klausel
  9,  0,Unerwartetes WHEN oder OTHERWISE
  9,  1,WHEN hat kein zugeh�riges SELECT
  9,  2,OTHERWISE hat kein zugeh�riges SELECT
 10,  0,Unerwartetes oder unpassendes END
 10,  1,END hat kein zugeh�riges DO oder SELECT
 10,  2,Dem END passend zum DO in Zeile %d muss ein Symbol passend f�r eine Laufvariable folgen (oder ganz ohne sein); "%s" gefunden|<linenumber>,<token>
 10,  3,Dem END passend zum DO in Zeile %d darf kein Symbol folgen, da keine Laufvariablen existieren; "%s" gefunden|<linenumber>,<token>
 10,  4,Dem END passend zum SELECT in Zeile %d darf kein Symbol folgen; "%s" gefunden|<linenumber>,<token>
 10,  5,END darf nicht direkt einem THEN folgen
 10,  6,END darf nicht direkt einem ELSE folgen
 11,  0,[Kontroll-Stack voll]
 12,  0,[Klausel hat mehr als 1024 Zeichen]
 13,  0,Ung�ltiges Zeichen im Programm
 13,  1,Ung�ltiges Zeichen im Programm "('%x'X)"|<hex-encoding>
 14,  0,Unvollst�ndiges DO/SELECT/IF
 14,  1,DO-Anweisung erwartet ein passendes END
 14,  2,SELECT-Anweisung erwartet ein passendes END
 14,  3,THEN erwartet eine nachfolgende Anweisung
 14,  4,ELSE erwartet eine nachfolgende Anweisung
 15,  0,Ung�ltige Hexadezimal- oder Bin�r-Zeichenkette
 15,  1,Ung�ltige Position eines Leerzeichens an Position %d in der Hexadezimal-Zeichenkette|<position>
 15,  2,Ung�ltige Position eines Leerzeichens an Position %d in der Bin�r-Zeichenkette|<position>
 15,  3,Nur 0-9, a-f, A-F und Leerzeichen sind in der Hexadezimal-Zeichenkette g�ltig; "%c" gefunden|<char>
 15,  4,Nur 0, 1 und Leerzeichen sind in der Bin�r-Zeichenkette g�ltig; "%c" gefunden|<char>
 16,  0,Sprungziel nicht gefunden
 16,  1,Sprungziel "%s" nicht gefunden|<name>
 16,  2,Kann kein SIGNAL zum Sprungziel "%s" machen, da es innerhalb einer IF, SELECT oder DO-Gruppe liegt|<name>
 16,  3,Kann das Sprungziel "%s" nicht aufrufen, da es innerhalb einer IF, SELECT oder DO-Gruppe liegt|<name>
 17,  0,Unerwartetes PROCEDURE
 17,  1,PROCEDURE ist nur g�ltig, wenn es die erste Anweisung nach einem internen CALL- oder Funktionsaufruf ist
 18,  0,THEN erwartet
 18,  1,IF-Schl�sselwort in Zeile %d erwartet eine zugeh�rige THEN-Klausel; "%s" gefunden|<linenumber>,<token>
 18,  2,WHEN-Schl�sselwort in Zeile %d erwartet eine zugeh�rige THEN-Klausel; "%s" gefunden|<linenumber>,<token>
 19,  0,Zeichenkette oder Symbol erwartet
 19,  1,Zeichenkette oder Symbol nach dem ADDRESS-Schl�sselwort erwartet; "%s" gefunden|<token>
 19,  2,Zeichenkette oder Symbol nach dem CALL-Schl�sselwort erwartet; "%s" gefunden|<token>
 19,  3,Zeichenkette oder Symbol nach dem NAME-Schl�sselwort erwartet; "%s" gefunden|<token>
 19,  4,Zeichenkette oder Symbol nach dem SIGNAL-Schl�sselwort erwartet; "%s" gefunden|<token>
 19,  6,Zeichenkette oder Symbol nach dem TRACE-Schl�sselwort erwartet; "%s" gefunden|<token>
 19,  7,Symbol in der Suchschablone erwartet; "%s" gefunden|<token>
 20,  0,Name erwartet
 20,  1,Name erwartet; "%s" gefunden|<token>
 20,  2,"%s" gefunden, wo nur ein Name g�ltig ist|<token>
 21,  0,Ung�ltige Daten am Ende der Klausel
 21,  1,Die Klausel endete an einem unerwarteten Token; "%s" gefunden|<token>
 22,  0,Ung�ltige Zeichenkette
 22,  1,Ung�ltige Zeichen in der Zeichenkette '%s'X|<hex-encoding>
 23,  0,Ung�ltige Daten-Zeichenkette
 23,  1,Ung�ltige Daten-Zeichenkette '%s'X|<hex-encoding>
 24,  0,Ung�ltige TRACE-Anforderung
 24,  1,Ein TRACE-Anforderungszeichen muss eines aus "%s" sein; "%c" gefunden|ACEFILNOR,<value>
 25,  0,Ung�ltiges Folge-Schl�sselwort gefunden
 25,  1,CALL ON fordert als Nachfolger eines der Schl�sselw�rter %s; "%s" gefunden|<keywords>,<token>
 25,  2,CALL OFF fordert als Nachfolger eines der Schl�sselw�rter %s; "%s" gefunden|<keywords>,<token>
 25,  3,SIGNAL ON fordert als Nachfolger eines der Schl�sselw�rter %s; "%s" gefunden|<keywords>,<token>
 25,  4,SIGNAL OFF fordert als Nachfolger eines der Schl�sselw�rter %s; "%s" gefunden|<keywords>,<token>
 25,  5,ADDRESS WITH fordert als Nachfolger eines der Schl�sselw�rter INPUT, OUTPUT or ERROR; "%s" gefunden|<token>
 25,  6,INPUT fordert als Nachfolger eines der Schl�sselw�rter STREAM, STEM, LIFO, FIFO oder NORMAL; "%s" gefunden|<token>
 25,  7,OUTPUT fordert als Nachfolger eines der Schl�sselw�rter STREAM, STEM, LIFO, FIFO, APPEND, REPLACE or NORMAL; "%s" gefunden|<token>
 25,  8,APPEND fordert als Nachfolger eines der Schl�sselw�rter STREAM, STEM, LIFO oder FIFO; "%s" gefunden|<token>
 25,  9,REPLACE fordert als Nachfolger eines der Schl�sselw�rter STREAM, STEM, LIFO oder FIFO; "%s" gefunden|<token>
 25, 11,NUMERIC FORM fordert als Nachfolger eines der Schl�sselw�rter %s; "%s" gefunden|<keywords>,<token>
 25, 12,PARSE fordert als Nachfolger eines der Schl�sselw�rter %s; "%s" gefunden|<keywords>,<token>
 25, 13,UPPER fordert als Nachfolger eines der Schl�sselw�rter %s; "%s" gefunden|<keywords>,<token>
 25, 14,ERROR fordert als Nachfolger eines der Schl�sselw�rter STREAM, STEM, LIFO, FIFO, APPEND, REPLACE or NORMAL; "%s" gefunden|<token>
 25, 15,NUMERIC fordert als Nachfolger eines der Schl�sselw�rter %s; "%s" gefunden|<keywords>,<token>
 25, 16,FOREVER fordert als Nachfolger eines der Schl�sselw�rter %s; "%s" gefunden|<keywords>,<token>
 25, 17,PROCEDURE fordert als Nachfolger eines der Schl�sselw�rter EXPOSE; "%s" gefunden|<token>
 26,  0,Ung�ltige ganze Zahl
 26,  1,Ganze Zahlen m�ssen innerhalb der DIGITS-Begrenzung(%d) bleiben; "%s" gefunden|<value>,<value>
 26,  2,Der Wert des Ausdrucks f�r den Wiederholungsz�hler der DO-Anweisung muss Null oder eine positive ganze Zahl sein; "%s" gefunden|<value>
 26,  3,Der Wert des FOR-Ausdrucks der DO-Anweisung muss Null oder eine positive ganze Zahl sein; "%s" gefunden|<value>
 26,  4,Positionsparameter der Suchschablone muss eine ganze Zahl sein; "%s" gefunden|<value>
 26,  5,NUMERIC DIGITS-Wert muss eine positive ganze Zahl sein; "%s" gefunden|<value>
 26,  6,NUMERIC FUZZ-Wert muss Null oder eine positive ganze Zahl sein; "%s" gefunden|<value>
 26,  7,Zahl bei der TRACE-Setzung muss eine ganze Zahl sein; "%s" gefunden|<value>
 26,  8,Operand auf der rechten Seite des Potenzierungsoperators ("**") muss eine ganze Zahl sein; "%s" gefunden|<value>
 26, 11,Ergebnis der Operation %s %% %s ben�tigt eine Exponentialnotation beim aktuellen NUMERIC DIGITS-Wert %d|<value>,<value>,<value>
 26, 12,Ergebnis der %%-Operation, ben�tigt f�r die Operation %s // %s, ben�tigt eine Exponentialnotation beim aktuellen NUMERIC DIGITS-Wert %d|<value>,<value>,<value>
 27,  0,Ung�ltige DO-Syntax
 27,  1,Verbotene Benutzung des Schl�sselwortes "%s" in der DO-Klausel|<token>
 28,  0,Ung�ltiges LEAVE oder ITERATE
 28,  1,LEAVE ist nur in einer wiederholenden DO-Schleife erlaubt
 28,  2,ITERATE ist nur in einer wiederholenen DO-Schleife erlaubt
 28,  3,Das Symbol hinter LEAVE ("%s") muss entweder eine Laufvariable einer aktuellen DO-Schleife sein oder komplett fehlen|<token>
 28,  4,Das Symbol hinter ITERATE ("%s") muss entweder eine Laufvariable einer aktuellen DO-Schleife sein oder komplett fehlen|<token>
 29,  0,Umbegungsname zu lang
 29,  1,Umgebungsname ist l�nger als %d Zeichen; "%s" gefunden|#Limit_EnvironmentName,<name>
 30,  0,Name oder Zeichenkette zu lang
 30,  1,Name ist l�nger als %d Zeichen|#Limit_Name
 30,  2,Zeichenkette ist l�nger als %d Zeichen|#Limit_Literal
 31,  0,Name f�ngt mit Zahl oder "." an
 31,  1,Ein Wert kann nicht einer Zahl zugewiesen weerden; "%s" gefunden|<token>
 31,  2,Variablensymbol darf nicht mit einer Zahl beginnen; "%s" gefunden|<token>
 31,  3,Variablensymbol darf nicht mit einem "." beginnen; "%s" gefunden|<token>
 32,  0,[Ung�ltige Benutzung eines Stamms]
 33,  0,Ung�ltiges Ergebnis eines Ausdrucks
 33,  1,Wert von NUMERIC DIGITS "%d" muss gr��er sein als der von NUMERIC FUZZ "(%d)"|<value>,<value>
 33,  2,Wert von NUMERIC DIGITS "%d" darf %d nicht �berschreiten|<value>,#Limit_Digits
 33,  3,Ergebnis des Ausdrucks hinter NUMERIC FORM muss mit "E" oder "S" anfangen; "%s" gefunden|<value>
 34,  0,Logischer Wert nicht "0" oder "1"
 34,  1,Wert des Ausdrucks hinter dem IF-Schl�sselwort muss entweder "0" oder "1" sein; "%s" gefunden|<value>
 34,  2,Wert des Ausdrucks hinter dem WHEN-Schl�sselwort muss entweder "0" oder "1" sein; "%s" gefunden|<value>
 34,  3,Wert des Ausdrucks hinter dem WHILE-Schl�sselwort muss entweder "0" oder "1" sein; "%s" gefunden|<value>
 34,  4,Wert des Ausdrucks hinter dem UNTIL-Schl�sselwort muss entweder "0" oder "1" sein; "%s" gefunden|<value>
 34,  5,Wert des Ausdrucks zur Linken des logischen Operators "%s" muss entweder "0" oder "1"; "%s" gefunden|<operator>,<value>
 34,  6,Wert des Ausdrucks zur Rechten des logischen Operators "%s" muss entweder "0" oder "1"; "%s" gefunden|<operator>,<value>
 35,  0,Ung�ltiger Ausdruck
 35,  1,Ung�ltiger Ausdruck entdeckt bei "%s"|<token>
 36,  0,Fehlende schlie�ende Klammer ")" im Ausdruck
 37,  0,Unerwartetes "," oder ")"
 37,  1,Unerwartetes ","
 37,  2,")" ohne �ffnende Klammer im Ausdruck
 38,  0,Ung�ltige Suchschablone oder Muster
 38,  1,Ung�ltige Suchschablone in "%s"|<token>
 38,  2,Ung�ltige Suchposition in "%s"|<token>
 38,  3,PARSE VALUE-Anweisung erwartet das WITH-Schl�sselwort
 39,  0,[Berechnungs-Stack �bergelaufen]
 40,  0,Ung�ltiger Aufruf einer Routine
 40,  1,Aufruf der externen Routine "%s" schl�gt fehl|<name>
 40,  3,Nicht genug Argumente beim Aufruf von "%s"; das Minimum ist %d|<bif>,<argnumber>
 40,  4,Zu viele Argumente beim Aufruf von "%s"; das Maximum ist %d|<bif>,<argnumber>
 40,  5,Fehlendes Argument beim Aufruf von "%s"; Argument %d wird ben�tigt|<bif>,<argnumber>
 40,  9,Routine %s, Argument %d: Exponent �berschreitet %d Ziffern; "%s" gefunden|<bif>,<argnumber>,#Limit_ExponentDigits,<value>
 40, 11,Routine %s, Argument %d muss eine Zahl sein; "%s" gefunden|<bif>,<argnumber>,<value>
 40, 12,Routine %s, Argument %d muss eine ganze Zahl sein; "%s" gefunden|<bif>,<argnumber>,<value>
 40, 13,Routine %s, Argument %d muss Null oder positiv sein; "%s" gefunden|<bif>,<argnumber>,<value>
 40, 14,Routine %s, Argument %d muss positiv sein; "%s" gefunden|<bif>,<argnumber>,<value>
 40, 17,Routine %s, Argument 1 muss einen Ganzzahlteil im Bereich 0-90 Ziffern und einen Nachkommateil nicht gr��er als .9 haben; "%s" gefunden|<bif>,<value>
 40, 18,Routine %s, Umwandlung verlangt ein Jahr im Bereich 0001 bis 9999|<bif>
 40, 19,Routine %s, Argument 2, "%s", gen�gt nicht dem Format beschrieben in Argument 3, "%s"|<bif>,<value>,<value>
 40, 21,Routine %s, Argument %d darf nicht leer sein|<bif>,<argnumber>
 40, 23,Routine %s, Argument %d muss ein einzelnes Zeichen sein; "%s" gefunden|<bif>,<argnumber>,<value>
 40, 24,Routine %s, Argument 1 muss eine Bin�rzeichenkette sein; "%s" gefunden|<bif>,<value>
 40, 25,Routine %s, Argument 1 muss eine Hexadezimalzeichenkette sein; "%s" gefunden|<bif>,<value>
 40, 26,Routine %s, Argument 1 muss ein g�ltiges Symbol sein; "%s" gefunden|<bif>,<value>
 40, 27,Routine %s, Argument 1 muss ein g�ltiger Dateiname sein; "%s" gefunden|<bif>,<value>
 40, 28,Routine %s, Argument %d: Option muss mit einem Zeichen aus "%s" anfangen; "%s" gefunden|<bif>,<argnumber>,<optionslist>,<value>
 40, 29,Routine %s, Umwandlung zum Format "%s" ist nicht erlaubt|<bif>,<value>
 40, 31,Routine %s, Argument 1 ("%d") darf 100000 nicht �berschreiten|<bif>,<value>
 40, 32,Routine %s, die Differenz zwischen Argument 1 ("%d") und Argument 2 ("%d") darf 100000 nicht �berschreiten|<bif>,<value>,<value>
 40, 33,Routine %s, Argument 1 ("%d") muss kleiner oder gleich Argument 2 ("%d") sein|<bif>,<value>,<value>
 40, 34,Routine %s, Argument 1 ("%d") muss kleiner oder gleich der Anzahl der Zeilen im Programm (%d) sein|<bif>,<value>,<sourceline()>
 40, 35,Routine %s, Argument 1 kann nicht als ganze Zahl dargestellt werden; "%s" gefunden|<bif>,<value>
 40, 36,Routine %s, Argument 1 muss der Name einer Variablen im Namensraum sein; "%s" gefunden|<bif>,<value>
 40, 37,Routine %s, Argument 3 muss der Name eines Namensraums sein; "%s" gefunden|<bif>,<value>
 40, 38,Routine %s, Argument %d ist nicht gro� genug, um "%s" darzustellen|<bif>,<argnumber>,<value>
 40, 39,Routine %s, Argument 3 ist weder Null noch Eins; "%s" gefunden|<bif>,<value>
 40, 41,Routine %s, Argument %d muss innerhalb der Dateigrenzen sein; "%s" gefunden|<bif>,<argnumber>,<value>
 40, 42,Routine %s, Argument 1: auf dieser Datei kann nicht positioniert werden; "%s" gefunden|<bif>,<value>
 40,914,[Routine %s, Argument %d: muss ein Wert aus "%s" sein; gefunden: "%s"]|<bif>,<argnumber>,<optionslist>,<value>
 40,920,[Routine %s: Systemfehler bei der Ein-/Ausgabe auf der Datei; %s]|<bif>,<description>
 40,921,[Routine %s, Argument %d: Dateipositionierung im Modus "%s"; nicht kompatibel mit dem �ffnungsmodus]|<bif>,<argnumber>,<value>
 40,922,[Routine %s, Argument %d: Zuwenig Folgebefehle; das Minimum ist %d; gefunden %d]|<bif>,<argnumber>,<value>,<value>
 40,923,[Routine %s, Argument %d, Zuviele Folgebefehle; das Maximum ist %d; gefunden %d]|<bif>,<argnumber>,<value>,<value>
 40,924,[Routine %s, Argument %d, ung�ltige Positionierungsbasis; erwartet wird ein Wort aus "%s"; gefunden "%s"]|<bif>,<argnumber>,<value>,<value>
 40,930,[Routine RXQUEUE, Funktion TIMEOUT: Ein Ganzzahlwert zwischen -1 und %d erwartet; gefunden: \"%s\"]|<value>,<value>
 40,980,Unerwartete Eingabe, entweder unbekannter Typ oder nicht erlaubte Daten%s%s|: ,<location>
 40,981,Zahl au�erhalb des erlaubten Bereichs%s%s|: ,<location>
 40,982,Zeichenkette zu lang f�r den definierten Puffer%s%s|: ,<location>
 40,983,Illegale Kombination von Typ/Gr��e%s%s|: ,<location>
 40,984,Nicht unterst�tzte Zahl wie NAN, +INF, -INF%s%s|: ,<location>
 40,985,Struktur zu komplex f�r den statischen internen Puffer%s%s|: ,<location>
 40,986,Ein Element in der Struktur fehlt%s%s|: ,<location>
 40,987,Ein Wert in der Struktur fehlt%s%s|: ,<location>
 40,988,Der Name oder ein Namensteil ist ung�ltig f�r den Interpreter%s%s|: ,<location>
 40,989,Ein Problem tauchte auf bei der Schnittstelle zwischen Regina und GCI%s%s|: ,<location>
 40,990,Der Typ passt nicht zu den Anforderungen an Basistypen (Argumente/R�ckgabewert)%s%s|: ,<location>
 40,991,Die Anzahl der Argument ist falsch oder ein Argument fehlt%s%s|: ,<location>
 40,992,GCI's interner Argumentstack lief �ber%s%s|: ,<location>
 40,993,GCI z�hlte zu viele geschachtelte LIKE-Container%s%s|: ,<location>
 41,  0,Fehlerhafte Zahlumwandlung
 41,  1,Nichtnumerischer Wert ("%s") zur Linken des arithmetischen Operators "%s"|<value>,<operator>
 41,  2,Nichtnumerischer Wert ("%s") zur Rechten des arithmetischen Operators "%s"|<value>,<operator>
 41,  3,Nichtnumerischer Wert ("%s") mit dem Pr�fix-Operator "%s"|<value>,<operator>
 41,  4,Wert des TO-Ausdrucks in der DO-Anweisung muss numerisch sein; "%s" gefunden|<value>
 41,  5,Wert des BY-Ausdrucks in der DO-Anweisung muss numerisch sein; "%s" gefunden|<value>
 41,  6,Wert des Laufvariablen-Ausdrucks der DO-Anweisung muss numerisch sein; "%s" gefunden|<value>
 41,  7,Exponent �berschreitet %d Ziffern; "%s" gefunden|#Limit_ExponentDigits,<value>
 42,  0,Arithmetischer �ber-/Unterlauf
 42,  1,Arithmetischer �berlauf erkannt bei "%s %s %s"; Exponent des Ergebnisses ben�tigt mehr als %d Stellen|<value>,<operator>,<value>,#Limit_ExponentDigits
 42,  2,Arithmetischer Unterlauf erkannt bei "%s %s %s"; Exponent des Ergebnisses ben�tigt mehr als %d Stellen|<value>,<operator>,<value>,#Limit_ExponentDigits
 42,  3,Arithmetischer �berlauf; Nenner darf nicht Null sein
 43,  0,Routine nicht gefunden
 43,  1,Kann die Routine "%s" nicht finden|<name>
 44,  0,Funktion liefert keine Daten zur�ck
 44,  1,Funktion "%s" liefert keine Daten zur�ck|<name>
 45,  0,Keine Daten angegeben beim RETURN der Funktion
 45,  1,Daten erwartet bei der RETURN Anweisung, da die Routine "%s" als Funktion aufgerufen wurde|<name>
 46,  0,Ung�ltige Variablenangabe
 46,  1,Zusatztext ("%s") in einer Variablenangabe gefunden; ")" erwartet|<token>
 47,  0,Unerwartetes Sprungziel
 47,  1,INTERPRET darf keine Sprungziele enthalten; "%s" gefunden|<name>
 48,  0,Fehler bei einem Systemaufruf
 48,  1,Fehler bei einem Systemaufruf: %s|<description>
 48,920,Systemfehler bei der Ein-/Ausgabe auf der Datei: %s %s: %s|<description>,<stream>,<description>
 49,  0,Interpretationsfehler
 49,  1,Interpretationsfehler: Fehler in %s, Zeile %d: "%s". Bitte dem Fehler melden!|<module>,<linenumber>,<description>
 50,  0,Unerkanntes reserviertes Symbol
 50,  1,Unerkanntes reserviertes Symbol "%s"|<token>
 51,  0,Ung�ltiger Funktionsname
 51,  1,Punkt als Funktionsnamensende nicht erlaubt, ggf. Anf�hrungszeichen oder Hochkommata benutzen; "%s" gefunden|<token>
 52,  0,Ergebnis geliefert durch "%s" is l�nger als %d Zeichen|<name>,#Limit_String
 53,  0,Ung�ltige Option
 53,  1,Zeichenkette oder Symbol erwartet nach dem STREAM-Schl�sselwort; "%s" gefunden|<token>
 53,  2,Variablenname erwartet nach dem STEM-Schl�sselwort; "%s" gefunden|<token>
 53,  3,Argument zu STEM muss einen Punkt als letztes Zeichen haben;"%s" gefunden|<name>
 53,100,Zeichenkette oder Symbol erwartet nach dem LIFO-Schl�sselwort; "%s" gefunden|<token>
 53,101,Zeichenkette oder Symbol erwartet nach dem FIFO-Schl�sselwort; "%s" gefunden|<token>
 54,  0,Ung�ltiger STEM-Name
 54,  1,F�r diesen STEM APPEND-Befehl muss "%s" als Wert eine Zeilenanzahl enthalten; "%s" gefunden|<name>,<value>
#
# Alle Fehlermeldungen nach dieser Zeile sind nicht durch ANSI definiert.
#
 60,  0,[Kann eine Pseudo-Datei nicht auf den Anfang positionieren]
 61,  0,[Ung�ltige Positionieroperation auf einer Datei]
 64,  0,[Syntaxfehler w�hrend der Satzanalyse]
 64,  1,[Syntaxfehler in Zeile %d]
 64,  2,[Allgemeiner Sytaxfehler in Zeile %d, Spalte %d]|<linenumber>,<columnnumber>
 90,  0,[Nicht-ANSI-Besonderheit mit "OPTIONS STRICT_ANSI" benutzt]
 90,  1,[%s ist eine Erweiterungs-BIF von Regina (BIF = built in function, Standardfunktion)]|<bif>
 90,  2,[%s ist eine Erweiterungsinstruktion von Regina]|<token>
 90,  3,[Routine %s, Argument %d: Option muss ein Wert aus "%s" sein, wenn "OPTIONS STRICT_ANSI" vorliegt; Regina-Erweiterung "%s" gefunden]|<bif>,<argnumber>,<optionslist>,<value>
 93,  0,[Ung�ltiger Aufruf einer Routine]
 93,  1,[Das STREAM-Kommando %s fordert als Nachfolger eines der Worte "%s"; "%s" gefunden]|<token>,<value>,<value>
 93,  3,[Das STREAM-Kommando muss eines sein aus "%s"; "%s" gefunden]|<value>,<value>
 94,  0,[Schnittstellenfehler mit einer externen Warteschlange]
 94, 99,[Interner Fehler mit der Schnittstelle der externen Warteschlange: %d "%s"]|<description>,<systemerror>
 94,100,[Allgemeiner Systemfehler mit der Schnittstelle der externen Warteschlange. %s. %s]|<description>,<systemerror>
 94,101,[Fehler beim Verbinden mit %s auf Portnummer %d: "%s"]|<machine>,<portnumber>,<systemerror>
 94,102,[Kann die IP-Adresse von %s nicht bestimmen]|<machine>
 94,103,[Ung�ltiges Format f�r den Server im angegebenen Warteschlangennamen: "%s"]|<queuename>
 94,104,[Ung�ltiges Format f�r den Warteschlangennamen: "%s"]|<queuename>
 94,105,[Kann die Schnittstelle zu den Windows-Sockets nicht hochfahren: %s]|<systemerror>
 94,106,[H�chstanzahl f�r externe Warteschlangen �berschritten: %d]|<maxqueues>
 94,107,[Fehler w�hrend des Lesens eines Sockets: %s]|<systemerror>
 94,108,[Ung�ltiger Schalter weitergegeben. Es muss einer aus "%s" sein.]|<switch>
 94,109,[Queue \"%s\" nicht gefunden]|<queuename>
 94,110,[Routine %s ung�ltig bei externen Queues]|<bif>
 94,111,[Routine RXQUEUE, Funktion %s ung�ltig bei internen Queues]|<functionname>
 95,  0,[Gesperrte Routine im "gesch�tzten" Modus benutzt]
 95,  1,[Routine %s ung�ltig im "gesch�tzten" Modus]|<token>
 95,  2,[Routine %s, Argument %d ung�ltig im "gesch�tzten" Modus]|<bif>,<argnumber>
 95,  3,[Routine %s, Argument %d: "%s" ung�ltig im "gesch�tzten" Modus]|<bif>,<argnumber>,<token>
 95,  4,[Routine STREAM, Argument 3: �ffnen von Dateien mit "WRITE"-Zugriff ung�ltig im "gesch�tzten" Modus]
 95,  5,[Ausf�hren externer Kommandos ist im "gesch�tzten" Modus nicht erlaubt]
100,  0,[Unbekannter Dateisystemfehler]
