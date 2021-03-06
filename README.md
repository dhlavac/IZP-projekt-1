# IZP-projekt-1
FIT VUTBR, predmet IZP 2014/2015 prvý projekt,

Hodnotenie 5/5

Zadanie:

Popis projektu

Implementujte jednoduchý tabulkový kalkulátor. Program bude implementovat funkce vyhledání maxima, minima, funkce součtu a aritmetického průměru vybraných buněk. Tabulku ve formě textového souboru bude program očekávat na standardním vstupu. Požadovanou operaci a výběr buněk specifikuje uživatel v argumentu příkazové řádky.

Argumenty programu:

--help způsobí, že program vytiskne nápovědu používání programu a skončí.
operace reprezentuje jednu z následujících operací:
    select značí operaci, která z dané tabulky pouze vybere a následně vytiskne hodnoty daných buněk,
    min značí vyhledání a následný tisk minimální hodnoty z daného rozsahu buněk,
    max značí vyhledání a následný tisk maximální hodnoty z daného rozsahu buněk,
    sum značí výpočet a následný tisk sumy hodnot všech vybraných buněk,
    avg značí výpočet a následný tisk aritmetického průměru vybraných buněk.
výběr_buněk reprezentuje rozsah tabulky, nad kterým má být provedena daná operace. Rozsah je definován 
jedním z následujících argumentů:
    row X značí výběr všech buněk na řádku X (X > 0),
    col X značí výběr všech buněk ve sloupci X (X > 0),
    rows X Y značí výběr všech buněk od řádku X (včetně) až po Y (včetně). 0 < X <= Y.
    cols X Y značí výběr všech buněk od sloupce X (včetně) až po Y (včetně). 0 < X <= Y.
    range A B X Y značí výběr buněk od řádku A po řádek B a od sloupce X po sloupec Y (včetně daných 
    řádků a sloupců). 0 < A <= B, 0 < X <= Y.
    
    
    Implementační detaily

Program čte zpracovávanou tabulku ze standardního vstupu (stdin). Veškeré informace vypisuje na standardní výstup (stdout). Řádek tabulky je zakončen znakem konce řádku. Sloupce tabulky (resp. buňky na řádku) jsou odděleny jedním a více bílých znaků. Buňka může obsahovat číslo nebo text (bez bílých znaků). Maximální délka jednoho řádku je 1024 znaků.

Operace a jejich výstup

Operace select vypíše seznam hodnot z daného výběru. Každá hodnota bude vypsána na jeden řádek. V případě, že hodnota je číslo, bude vypsáno formátem %.10g funkce printf. V případě, že výběr zahrnuje více řádků či sloupců, hodnoty tabulky budou zpracovávány po řádcích, tj. nejprve všechny hodnoty daného řádku zleva doprava,pak hodnoty následujícího řádku. Operace min, max, sum a avg pracují pouze nad buňkami obsahující číselné údaje. Funkce vypíší výsledek jako jedno číslo formátu %.10g na samostatný řádek.

Neočekávané chování

Na chyby za běhu programu reagujte obvyklým způsobem: Na neočekávaná vstupní data, argumenty příkazového řádku, formát vstupních dat nebo chyby při volání funkcí reagujte přerušením programu se stručným a výstižným chybovým hlášením na příslušný výstup a odpovídajícím návratovým kódem.
