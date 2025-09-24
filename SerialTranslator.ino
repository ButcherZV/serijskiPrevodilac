define POWER_PIN 9
define STATUS_PIN 8

String receivedMessage;
String sentMessage;

/*  Poruka koju uređaj prima: [1]T0000000250001000000001600010Profiglass doo  04.06.24.  300  x  300;
Ako ovo dobijamo kada očitavamo šta uređaj šalje preko Herculesa onda je potrebno je obrisati prva 32 karaktera sa inputa (linija 29);
Ako ne dobijaš željeni ispis na izlazu (Profiglass doo  04.06.24.  300  x  300), potrebno je izmeniti linije 30 i/ili 31, tu formatiraš
input komandu, tako da ti ostaje samo potrebna informacija, koju dalje formatiramo u ZTC sintaksu.

OBJAŠNJENJE KOMANDI
receivedMessage.remove(0,32) - brisanje prva 32 karaktera iz primljenje poruke (u ovom slučaju brisanje ovog dela teksta [1]T0000000250001000000001600010) iz primljene komande;
                               ako je potrebno povećaj ili smanji broj 32 u zavisnosti da li treba da obrišeš više ili manje eksta na početku komande;
                               prvi broj 0 predstavlja indeks pozicije karaktera od kojeg krećemo da brišemo određenu količinu teksta, u našem slučaju to je 32, tako da 32
                               karaktera će biti obrisana sa početka teksta (računari broje od 0, tako da indeks prvog karaktera je 0, a ne 1)
receivedMessage.remove(receivedMessage.length()-1,1) - brisanje zadnjeg karaktera iz poruke; pošto je komanda koju primamo promenjive dužine, nekad ne znamo kolika je tačna dužina
                                                       teksta,  pa koristimo receivedMessage.length() da bi saznali lokaciju zadnjeg karaktera i obrisali. Često uređaji dobijaju
                                                       zvezdicu (*) na kraju komande, pa sa ovom komandom brišem tu zvezdicu sa ispisa, ako nemate potrebu za ovim onda samo
                                                       dodajte // na početku ove komande da bi je isključili (npr. //receivedMessage.remove(receivedMessage.length()-1,1))
*/


void setup() {
  Serial.begin(9600);             // Debug Serial
  Serial1.begin(9600);            // Inicijalizacija Serial1 porta za INPUT podataka (Pin 19 i 18)
  Serial2.begin(9600);            // Inicijalizacija Serial2 porta za OUTPUT podataka (Pin 17 i 16)
  pinMode(POWER_PIN, OUTPUT);
  pinMode(SATUS_PIN, OUTPUT);
  delay(1000);                    // Sačekaj sekundu da bi bili sigurni da je Serijska komunikacija inicijalizovana
  digitalWrite(POWER_PIN, HIGH);  // Uključi crveni LED da se vidi da je kutija pod naponom
}

void loop() {
  if (Serial1.available()) {
    digitalWrite(STATUS_PIN, HIGH);                         // Status LED uključen
    receivedMessage = Serial1.readString();                 // Prijem informacija
    Serial.println("PRIMLJENO: " + receivedMessage);        // Debug
    receivedMessage.trim();                                 // Brisanje nepotrebnog whitespacea i kontrolnih kodova
    receivedMessage.remove(0,32);                           // Brisanje prva 32 karaktera (0 označava lokaciju slova od kojeg se briše, a 32 označava koliko karaktera se briše)
    receivedMessage.remove(receivedMessage.length()-1,1);   // Brisanje zadnjeg karaktera, kojeg štampač obično vidi kao zvezdicu *, ako nije potrebno dodati // na početku ove linije
    sentMessage = "JDA|TXT=" + receivedMessage;             // Ubacivanje ZTC koda na tekst
    Serial2.println(sentMessage);                           // Slanje ispisa na štampač
    Serial.println("POSLANO: " + sentMessage);              // Debug
    digitalWrite(STATUS_PIN, LOW);                          // Satus LED isključen
    }
}