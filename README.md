# PP7

## Goal

In this exercise you will:

* **Understand** each stage of the C compilation pipeline (preprocessing, compilation to assembly, assembling, and linking).
* **Inspect** intermediate outputs to see how your C code transforms at each step.
* **Use** text‑processing tools (`grep`, `sed`, `awk`) with regular expressions to search and replace patterns in code.
* **Automate** search/replace interactively and non‑interactively in **Vim**.
* **Modularize** C code by defining functions across separate files and **link** them together manually.

**Important:** Start a stopwatch when you begin and work uninterruptedly for **90 minutes**. When time is up, stop immediately and record exactly where you paused.

---

## Workflow

1. **Fork** this repository on GitHub.
2. **Clone** your fork to your local machine.
3. Create a directory at the repository root named `solutions/`:

   ```bash
   mkdir solutions
   ```
4. For each task below, add the required files under `solutions/` (for example, `solutions/sample.c`, `solutions/sample.i`, etc.).
5. **Commit** your changes locally, then **push** them to GitHub.
6. **Submit** your repository link for review.

---

## Prerequisites

* A C compiler toolchain (e.g. `gcc`).
* Familiarity with command‑line tools: `grep`, `sed`, `awk`, and `vim`.
* Man‑pages for reference:

  ```bash
  man gcc   # for compiler options
  man grep  # for searching
  man sed   # for stream editing
  man awk   # for pattern scanning
  man vim   # for interactive editing
  ```

---

## Tasks

### Task 1: C Compilation Stages

**Objective:** Break down the process of transforming C source into an executable and inspect each intermediate file.

1. In `solutions/`, create a C source file named `sample.c` containing a simple `main()`:

   ```c
   #include <stdio.h>

   int main(void) {
       printf("Hello, PP7!\n");
       return 0;
   }
   ```
2. **Preprocess** only (run the preprocessor, `-E`) and save the output:

   ```bash
   gcc -E sample.c -o solutions/sample.i
   ```

   * Open `solutions/sample.i` in an editor and note how `#include <stdio.h>` expands and macros are handled.
3. **Compile to assembly** (`-S`):

   ```bash
   gcc -S solutions/sample.i -o solutions/sample.s
   ```

   * Examine `solutions/sample.s` to see the generated assembly instructions for `printf` and `return`.
4. **Assemble** (`-c`):

   ```bash
   gcc -c solutions/sample.s -o solutions/sample.o
   ```

   * Verify that `solutions/sample.o` is an object file (e.g., with `file sample.o`).
5. **Link** to produce an executable:

   ```bash
   gcc solutions/sample.o -o solutions/sample
   ```

   * Run `./solutions/sample` and confirm it prints `Hello, PP7!`.
6. **Explain** in comments or a short README how each stage transforms the code.

   A) Präprozessierung: Die erste Stufe `gcc -E sample.c -o sample.i` löst Direktiven wie `#include` und `#define` auf. Ergebnis: ein reiner C-Quelltext mit vollständigem Code aus z. B. `stdio.h`
   B) Kompilierung: Die zweite Stufe `gcc -S sample.i -o sample.s` übersetzt den C-Code in Assembly. Ergebnis: Menschenlesbarer Assembler-Code mit Funktionsaufrufen wie `call` oder `movl`
   C) Assemblierung: Die dritte Stufe `gcc -c sample.s -o sample.o` wandelt den Assembler-Code in Objekt-Code um. Ergebnis: Binäre Datei mit Maschinenbefehlen, aber noch nicht ausführbar.
   D) Linking: Die vierte Stufe `gcc sample.o -o sample` verbindet die Objektdatei mit Standardbibliotheken (z. B. libc, in der printf definiert ist). Ergebnis: Vollständig ausführbares Programm.
   E) Ausführen: `./sample` führt das Programm aus

---

### Task 2: Regex Search & Replace in Code

**Objective:** Use `grep`, `sed`, and `awk` to locate and modify code patterns, then perform the same edits in **Vim** both interactively and via the CLI.

1. Copy `solutions/sample.c` to `solutions/debug_sample.c`:

   ```bash
   cp solutions/sample.c solutions/debug_sample.c
   ```
2. **Search** for all calls to `printf` using `grep` with a regex pattern and save the results:

   ```bash
   grep -En "printf\s*\(" solutions/debug_sample.c
   ```
3. **Replace** each `printf` with `debug_printf` in the file using `sed` (in‑place):

   ```bash
   sed -E -i.bak 's/printf\s*/debug_printf/g' solutions/debug_sample.c
   ```
4. **Extract** lines containing `debug_printf` using `awk`:

   ```bash
   awk '/debug_printf/ { print NR, $0 }' solutions/debug_sample.c
   ```
5. **Vim Interactive**: open `solutions/debug_sample.c` in Vim:

   ```bash
   vim solutions/debug_sample.c
   ```

   * Use search (`/printf`) and substitute (`:%s/printf/debug_printf/g`) commands interactively.
   * Save and quit.
6. **Vim CLI**: perform the same substitute without opening the full UI:

   ```bash
   vim -c ":%s/printf/debug_printf/g" -c ":wq" solutions/debug_sample.c
   ```
7. **Explain** each tool’s approach to regex-based search and replace, and when you might prefer one over the others.

   A) `grep` - search only
   Ansatz: `grep` durchsucht Dateien nach Mustern, die mit regulären Ausdrücken beschrieben sind. Es gibt nur Trefferzeilen aus, ohne Änderungen am Inhalt vorzunehmen.
   Bevorzugt, wenn: man schnell nach Code- oder Textmustern suchen will, z. B. um alle Funktionsaufrufe zu finden oder zu überprüfen, ob ein Pattern überhaupt vorhanden ist.

   B) `sed` - search and replace
   Ansatz: `sed` durchsucht Textzeilen mit regulären Ausdrücken und kann diese inline modifizieren. Es eignet sich hervorragend für automatisierte Ersetzungen.
   Bevorzugt, wenn: man viele Dateien automatisiert bearbeiten möchte.

   C) `awk` - search & process
   Ansatz: `awk` kombiniert Pattern Matching mit Aktionen. Es kann Zeilen mit einem bestimmten Regex erkennen und dann benutzerdefinierte Operationen (z. B. Ausgaben, Zählen, Berechnungen) ausführen.
   Bevorzugt, wenn: man zeilenweise analysieren oder filtern möchte, z. B. Treffer mit Zeilennummer ausgeben oder bestimmte Muster statistisch auswerten will.

   D) `vim` Interactive or scripted search & relpace
   Ansatz: `vim` erlaubt sowohl interaktive als auch skriptgesteuerte Suchen und Ersetzungen mit regulären Ausdrücken. Die Befehle ähneln sed, sind aber in einer Texteditor-Umgebung eingebettet.
   Bevorzugt, wenn: man Änderungen visuell kontrollieren möchte oder komplexere Refactorings mit kontextbezogenen Entscheidungen durchführen will.

---

### Task 3: Modular Linking with `extern`

**Objective:** Create a separate addition function in `add.c`, declare it `extern` in `main.c`, and compile/link manually to form an executable.

1. In `solutions/`, create `add.c`:

   ```c
   // add.c
   int add(int a, int b) {
       return a + b;
   }
   ```
2. Create `main.c` that uses `add`:

   ```c
   // main.c
   #include <stdio.h>
   extern int add(int, int);

   int main(void) {
       int sum = add(5, 7);
       printf("5 + 7 = %d\n", sum);
       return 0;
   }
   ```
3. **Compile** each source separately:

   ```bash
   gcc -c solutions/add.c -o solutions/add.o
   gcc -c solutions/main.c -o solutions/main.o
   ```
4. **Link** the object files manually:

   ```bash
   gcc solutions/add.o solutions/main.o -o solutions/add_example
   ```
5. Run `./solutions/add_example` to verify it prints `5 + 7 = 12`.
6. **Document** the workflow in comments or a short file, emphasizing:

   * The role of `extern` declarations.
   * Why separating compilation can speed up builds.
   * How manual linking differs from letting `gcc` handle all steps in one command.

   A) Role of extern:
   Das Schlüsselwort „extern“ in „main.c“ gibt an, dass die Funktion „add“ in einer anderen Übersetzungseinheit (add.c) definiert ist. Dies ermöglicht dem Compiler eine Typprüfung der   Verwendung, ohne die vollständige Definition einsehen zu müssen.

   B)Benefits of separate compilation:
   - Nur geänderte Quelldateien müssen neu kompiliert werden.
   - Schnellere Builds, insbesondere bei großen Projekten.
   - Ermöglicht die Wiederverwendung kompilierter Objektdateien in verschiedenen Programmen.

   C) Manual linking:
   - Durch die Verwendung von „gcc -c“ wird jede Quelldatei ohne Verknüpfung in eine „.o“-Objektdatei kompiliert.
   - Dann verknüpft der letzte „gcc“-Aufruf diese Objektdateien zu einer einzigen ausführbaren Datei.
   - Dies ermöglicht eine bessere Kontrolle über den Erstellungsprozess (z. B. welche Dateien verknüpft werden sollen, in welcher Reihenfolge usw.).
   - Im Gegensatz dazu übernimmt ein Ein-Schritt-Befehl wie „gcc main.c add.c -o add_example“ die Kompilierung und Verknüpfung automatisch, jedoch ohne Modularität.


---

**Remember:** Stop working after **90 minutes** and record where you stopped.
