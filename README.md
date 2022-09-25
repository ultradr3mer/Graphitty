## SoSe2022 Praktikum C++ für JAVA Programmierer
# Graphitty

## Einleitung
### Zweck und Ziele des Programms
Das Programm von uns dient zur Berechnung und Darstellung diverser Funktionen und Methoden der Betriebswirtschaft zur Auswertung von Produktions- und Kostendaten, um Produktions-, Kosten- und Betriebsaufwände zu minimieren. Gängige Funktionen, Berechnungen und Anwendungen aus dem Studienmodul von “BWL II” werden dabei in das Programm integriert.

Die Simulation wird als Programm in C++ umgesetzt und mit einer grafischen Oberfläche integriert, um die Auswertung und Darstellung besser zu visualisieren und durch generierte Graphen und Schaubilder aus den gegebenen Eingaben und Benutzeranforderungen veranschaulicht.
### Benutzer des Programms
Leute im Management und im Studium von BWL. Einerseits können von hand berechnete Ergebnisse verglichen werden, aber auch komplexere Formeln einfach dargestellt und durchgeführt werden, welche im Anschluss visual dem Benutzer zur Verfügung gestellt werden.

### Annahmen und Abhängigkeiten
- Das Programm benötigt valide Inputs von den Benutzern, um Berechnungen durchführen zu können. Dafür werden Richtlinien zur richtigen Eingabe von Formeln, Gleichungen und Funktionen während der Entwicklung definiert und vorgegeben.
-Benutzer können neue Projekte, bzw. Arbeitsmappen über das Programm anlegen, öffnen und starten, um innerhalb dieser Arbeitsmappen die gewünschten Berechnungen erstellen, bearbeiten und anzeigen lassen zu können. Die verschiedenen Arbeiten von Berechnungen (/Inhalten) werden vom Programm vorgegeben.
- Die verschiedenen Arbeiten von Berechnungen geben einen jeweiligen Input vor, welche für die Funktionen benötigt wird, um die letztendlichen Berechnungen und das generieren der dazugehörigen Grafiken durchzuführen.
- Den Benutzern wird die Möglichkeit zur Verfügung gestellt, Berechnungen aus anderen Arbeitsmappen jederzeit zu importieren.
- Das Programm erhält eine Benutzeroberfläche zum Navigieren der Funktionen. Dazu zählen:
- Eine Hauptnavigation zum Wechseln, Erstellen und Verwalten von Arbeitsmappen
- Die Anzeige von Arbeitsmappen-Inhalten und deren dazugehörigen, angelegten Berechnungen als Listenansicht, zum einfachen Navigieren von durchgeführten Funktionen.
- Diverse Elemente zum Schnellbearbeiten von Daten (z.B. Importe & Exporte von Berechnungen, die Anzeige von zuletzt geöffneten Arbeitsmappen & Berechnungen und weiteres)
- Das Programm gibt dabei vor, in welchem Format die benötigten Formeln oder Funktionen für Berechnungen eingeben werden müssen. Diese Formate werden im Programm einsehbar sein.

## Programm-Anforderungen
### Funktionale Anforderungen
- Eingabe und Erkennung von mathematischen Funktionen durch Angabe des Funktionsterms in herkömmlicher Schreibweise. 
Es wird ein Interpreter zum Erkennen und Auswerten von mathematischen Funktionen geben, der bei allen  Kernfunktionen zum Einsatz kommt. Damit soll dem Nutzer die Möglichkeit gegeben werden, Funktionsausdrücke in der herkömmlichen Schreibweise einzugeben, also in der Schreibweise, die in der Mathematik und Betriebswirtschaftslehre üblich ist. Es können die Operatoren Plus (+), Minus (-), Multiplikation (*), Division (/) und die Potenzfunktion (^), sowie Klammern verwendet werden, wobei alle Rechenregeln wie “Punkt vor Strich” beachtet werden. Der Interpreter liest die Ausdrücke und stellt sie den  anderen Modulen des Programms so zur verfügung, dass diese damit durch einsetzen von Werten für Variablen das Ergebnis erhalten können und somit für weitere berechnungen, wie das Zeichnen von Graphen, genutzt werden können.
- Aufteilung Programmprojekte in Arbeitsmappen zur Indexierung und Zusammenführung von Programmaufgaben. 
Der Arbeitsbereich für Benutzer soll in sogenannte Arbeitsmappen aufgeteilt werden, in welchen die Benutzer ihre zukünftigen Berechnungen erstellen und abspeichern können. Jede Arbeitsmappe kann dabei eine beliebige Anzahl an Berechnungen haben. Durch Arbeitsmappen lässt sich das Entwicklungsprojekt besser für den Endnutzer aufteilen und gestalten, da Berechnungen somit nicht jeweils einzeln geöffnet werden müssen, sondern eine Zuweisung erhalten und zeitgleich bearbeitet und eingesehen werden können.
- Gestaltung und Bereitstellung einer Benutzeroberfläche fürs Navigieren, Aufrufen und Durchführen vorhandener Funktionen. 
Damit Nutzer das Programm auch richtig verwenden und bedienen können, wird eine Benutzeroberfläche benötigt. Die Benutzeroberfläche soll einen Bereich zum Verwalten von Arbeitsmappen besitzen (Erstellen/Löschen/Öffnen), sowie das Arbeitsmappen-Layout bereitstellen (siehe Konzept GUI), damit Benutzer neue Berechnungen erstellen und importieren, sowie vorhandene Berechnungen einsehen und bearbeiten können.
- Berechnung: Polynom-Generator, der die Funktionsgleichung eines Polynomes anhand beliebiger, valider, vom Benutzer ausgewählter Punkte bestimmt.
In der BWL kommt es vor, das bereits historische Messwerte existieren oder Zukünftige Werte Prognostiziert wurden. Mit dem Polynom Generator lässt sich einfach eine Funktion aufstellen welche durch die Messpunkte verläuft. Diese kann nach der generierung noch von Hand bearbeitet und angepasst werden.
- Berechnung: 4 Phasen Schema wird aus einer Produktionsfunktion ausgewertet. 
Dabei wird der Gesamtertrag, Grenzertrag, Grenzertragszuwachs und Durchschnittsertrag berechnet. Dann werden die Phasenübergänge III, IIIII, IIIIV.
Es ist aber auch möglich andere Funktionen als Produktionsfunktionen einzugeben. Beispielsweise kann eine Kostenfunktion eingegeben werden und auch das 4 Phasen Schema der Kosten ermittelt werden.
- Wechsel zwischen Kosten- und Ertragsfunktion implementieren.
Es gibt einen Schalter, der es ermöglicht, anhand der Ertragsfunktion die Werte der Kostenfunktion und umgekehrt anzuzeigen. Es kann also entweder die Ertragsfunktion eingegeben werden oder die Kostenfunktion angegeben werden und die Werte der andern Funktion ermittelt werden.
Auch hier wird das 4 Phasen Schema analysiert, allerdings das 4-Phasen-Schema der Kosten.
- Speichern & Laden von Arbeitsmappen 
Eine Sammlung von Ansichten und Funktionen kann vom Benutzer als Arbeitsmappe in Form einer Datei gespeichert / geladen werden. Beim Speichern werden alle relevanten Eckdaten (u.A Art und Input für die Berechnungen innerhalb der Arbeitsmappe) der Arbeitsmappen in eine externe Datei geschrieben, um die Daten für weitere Bearbeitung später durch das Laden im Programm wieder zur Verfügung zu stellen. Beim Laden dieser Dateien wird der Inhalt vom Programm ausgewertet und daraus die gleiche Arbeitsmappe generiert, welche zuvor gespeichert (und ggf. geschlossen) wurde. Dadurch, dass die Berechnungen meist aus einer eingegebenen Formal durchgeführt werden, reicht es, diese Ausgangsformel, so wie vom Nutzer getätigte Konfigurationen zu speichern, um die Ergebnisse aufrecht zu erhalten.
- Es gibt zusätzlich die Möglichkeit weitere Funktionen 
einzublenden. Beispielsweise können die Funktion für die fixen Gesamtkosten und die Funktion für die variablen Gesamtkosten zusätzlich via Checkbox berechnet und eingeblendet werden.
- Individualisierung der Grafischen Darstellung von Funktionen und Berechnungen
Der Nutzer kann frei entscheiden, wie groß der angezeigte Bereich von Funktionsgraphen ist. Damit können die Nutzer ihre gewünschten Ergebnisse grafisch besser hervorheben oder Lehrbereiche ausblenden, welche keine relevanten Daten erfassen.
- Individualisierung des Auswertungsbereichs von Funktionen 
Der Auswertungsbereich, also welche Elemente bei der Darstellung von Funktionen angezeigt werden, kann vom Nutzer frei eingestellt werden. Elemente, die gerade nicht benötigt werden können so ausgeblendet werden damit nur die für die jeweilige Aufgabe nötigen Elemente sichtbar sind. Die Funktionsansicht und das Menü zur Einstellung der sichtbaren Funktionen und Menüs zur berechnung weiterer Werte können einzeln ein- oder ausgeschaltet werden um die Oberfläche einfach und übersichtlich zu gestalten.
- Verlaufsfunktion, um zuletzt angezeigte Arbeitsmappen beim Öffnen des Programms aufzulisten. 
Dadurch müssen Arbeitsmappen nicht immer neu über das den File-Explorer manuell geöffnet werden, sobald man zwischen Arbeitsmappen wechseln möchte. Die Verlaufsfunktion stellt eine Auflistung bereit, welche alle zuletzt angesehenen und bearbeiten Arbeitsmappen bereitstellt und durch einen Klick wieder öffnet.
- Suche für die Verlaufsfunktion.
Es gibt ein Suchfeld im Start Dialog mit dem die Liste der zuletzt angezeigten Arbeitsmappen gefiltert wird. Dazu gibt es einen Button der die Suche selbst auslöst. Es kann aber auch mit Enter im Textfeld gesucht werden. 
- Autosave Funktion.
Während des Bearbeitens wird alle 5 minuten ein Autosave erzeugt, diese werden in einem Ordner gesammelt. Auch beim Verlassen des Programms wird ein Autosave erzeugt. Über den Button Wiederherstellen auf dem Startdialog kann das letzte Autosave geladen werden.
- Lokalisierung.
im Start Dialog kann die Sprache gewählt werden. Es stehen Deutsch und Englisch zur verfügung. Die Lokalisierung wird über die QT Funktionen Implementiert.
