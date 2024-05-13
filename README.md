# esp32-chat
um chat usando esp32 para hostear uma pagina

#configurando
é preciso editar as linhas que tem a definição de ssid e password para o wifi que você quer utilizar! após isso é só fazer upload do arquivo para o esp32, fique atento também de adicionar antes de fazer upload a biblioteca WebSockets by Markus, ela é o #include <WebSocketsServer.h> ...

# dentro do site
ao entrar no site vai ser pedido um nome, e é assim que você pode começar a enviar mensagens
a mensagem é enviada completa para o outro usuário que estiver no site, portanto o modo de enviar mensagens com imagens e videos é via HTML injection
