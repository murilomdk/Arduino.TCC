
#include <SPI.h>
#include <WiFi.h>

char NomeWifi[] = "redewifi";
char Senha[] = "murilo123";

int status = WL_IDLE_STATUS;

WiFiServer server(80);

String cabecalho;

void setup() {

  Serial.begin(9600);

  while (status != WL_CONNECTED) {
    Serial.print("Conectando na Rede: ");
    Serial.println(NomeWifi);
    status = WiFi.begin(NomeWifi, Senha);
    delay(1000);
  }

  mostraStatus();

  server.begin();

   Serial.println("- Servidor Web Iniciado -");

} // fim setup

void loop() {

  WiFiClient cliente = server.available();
  
  if (cliente) {
    Serial.println("- Novo Cliente -");
    Serial.println("  ");

    boolean linhaVazia = true;

    while (cliente.connected()) {
      if (cliente.available()) {
        char c = cliente.read();
        cabecalho += c;

        if (c == '\n' && linhaVazia) {          

          Serial.println("Cabeçalho = ");
          Serial.print(cabecalho);

          if (cabecalho.indexOf("YWRtaW46YWRtaW40NTY=") >= 0) { // admin:admin456

            cliente.println("HTTP/1.1 200 OK");
            cliente.println("Content-Type: text/html");
            cliente.println("Connection: close");
             cliente.println();
            Serial.println("Acesso Permitido!");
             
            if (cabecalho.indexOf("GET / HTTP/1.1") >= 0) {
              cliente.println("<!DOCTYPE HTML>");
              cliente.println("<html>");
              cliente.println("Senha OK - Acesso Permitido!");
              cliente.println("</html>");
            }  else {
              cliente.println("<!DOCTYPE HTML>");
              cliente.println("<html>");
              cliente.println("Acesso Permitido!");
              cliente.println("</html>");
            }

          } else {

            cliente.println("HTTP/1.1 401 Unauthorized");
            cliente.println("WWW-Authenticate: Basic realm=\"Secure\"");
            cliente.println("Content-Type: text/html");
            cliente.println();
            cliente.println("<html> <h2>bt cancel - login cancelado </h2></html>");
             Serial.println("Acesso Negado!");
          }
          cabecalho = "";
          break;
        }
        if (c == '\n') {
          linhaVazia = true;
        }
        else if (c != '\r') {
          linhaVazia = false;
        }
      }
    }

    delay(10);

    cliente.stop();
    Serial.println(" - Cliente Disconectado - ");
    Serial.println("____________________________________");
    Serial.println("  ");
  }

} // fim loop

void mostraStatus() {

  Serial.println(" __________ STATUS DA REDE __________ ");

  Serial.print("Rede Conectada: ");
  Serial.println(WiFi.SSID());


  IPAddress ip = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(ip);

  Serial.println("____________________________________");

};
