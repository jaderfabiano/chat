Chat

Simula um chat entre duas aplicações via console. Servidor desenvolvido em C e o cliente em Python. Utiliza o PubSub do Redis.

Dependências

  1. Redis: disponível em http://redis.io/
  2. Hiredis: disponível em https://github.com/redis/hiredis
  3. LibEvent: sudo apt-get install libevent-dev
  4. redis-py: sudo pip install redis
  5. Make para compilar o server


Exemplo:
  server --chat
  
  Na mesma máquina ou em uma máquina remota. Execute o client.
  
  python client --ip '127.0.0.1' --port 8567
  
  Começe a trocar mensagens... :)





TODO:
  Remover warnings de compilação no server.
  
  
  Adicionar integração com o gtalk.
