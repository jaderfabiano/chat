#!/usr/bin/python2.7

import socket
import sys
import threading
import select
import time
import uuid
import redis
'''
Classe responsavel por ler de forma asincrona as mensagens digitadas no console.
Parametros: 
Client-> ponte de conexao com o Redis
Channel-> canal onde as mensagens serao publicadas
myYd-> Id que identifica o proprietario da mensagem
'''
class Stdin (threading.Thread):
	def __init__(self, client, channel, myId):
		threading.Thread.__init__(self)
		self.keep = True
		self.myId = myId
		self.client = client
		self.channel = channel

	def stop(self):
		self.keep = False

	def run(self):
		while self.keep:
			readMessage = [sys.stdin]
			timeout = 0.01
			message = select.select(readMessage, [], [], timeout)[0]
			if message:
				for item in message:
					line = item.readline()
					if not line:
						pass
					elif line.rstrip():
						msg = self.myId + ' ' +line
						self.client.publish(self.channel, line)
			time.sleep(0.1)

'''
Classe responsavel por ouvir as mensagens publicadas no canal da conversa
client -> Conexao com o Redis
channel-> canal onde as mensagens serao capturadas
remoteId-> id que identifica o outro processo
'''
class ListenerChat(threading.Thread):
	def __init__(self, client, channel, remoteId):
		threading.Thread.__init__(self)
		self.myId        = 'clientPython-'+remoteId
		self.remoteId    = 'serverC-' + remoteId
		self.redisClient = client
		self.channel     = channel
		self.pubsub      = self.redisClient.pubsub()
		self.pubsub.subscribe(channel)

	def run (self):
		print '\n\t Chat Iniciado!!!\n'
		std = Stdin(self.redisClient, self.channel, self.myId)
		std.start()
		for item in self.pubsub.listen():
			data = str(item['data'])
			if data.find('fim') >= 0:
				self.pubsub.unsubscribe()
				print 'Chat Finalizado'
				std.stop()
				break
			msg  = ''
			try:
				if (data.find(self.myId) >= 0):
					msg = data.split(mmyId)[1]
				elif (data.find(self.remoteId) >= 0):
					msg = data.split(self.remoteId)[1]
					print '\t'+str(msg)
			except Exception as name:
				pass
'''
Classe responsavel por abrir uma conexao com o server via socket
ip-> ip da maquina onde o servidor esta rodando
port-> porta em que o servidor esta escutando
'''
class Connection:
	def __init__(self, ip, port):
		self.port    = int(port)
		self.ip      = ip
		self.client  = ''
		
	def initConnection(self):
		connected = False
		try:
			self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.client.connect((self.ip, self.port))
			connected = True
		except Exception as name:
			print 'Falha : ' + str(name)
		return connected				

	'''
	Envia para o servidor qual o canal onde as duas aplicacoes irao trocar informacoes

	'''
	def sendChannel(self, channel):
		try:
			self.client.send(channel)
		except Exception as name:
			print 'Falha : '+ str(name)
		data = self.client.recv(256)
		self.client.close()
		return data

'''
Classe que inicia a aplicacao
'''

class StartApp:
	def __init__(self):
		self.conex = ''
		self.ip    = ''
		self.port  = ''

	def checkNumberArguments(self, amount):
		if ( amount == 5):
			return True
		else:
			return False

	def checkPortIsNumber(self, port):
		isNumber = False
		try:
			portConvert = int(port)
			isNumber = True
		except Exception as name:
			pass
		return isNumber

	def initApp(self):
		if ( sys.argv[1] == '--ip'):
			self.ip = sys.argv[2]
		if ( sys.argv[3] == '--port'):
			self.port = sys.argv[4]
		if ( ( self.port != '') and ( self.ip != '' ) ):
			self.app()

	'''
	Gera um uuid para usar como canal da conversa.
	'''
	def app(self):
		self.conex = Connection(self.ip, self.port)
		if (self.conex.initConnection()):
			channel = str(uuid.uuid1())
			remoteId = self.conex.sendChannel(channel)
			try:
				clientRedis = redis.Redis()
				client = ListenerChat(clientRedis, channel, remoteId)
				client.start()
			except Exception as name:
				print 'Falha: ' + str(name)

	def help(self):
		print '\t############################'
		print'\t#      USAGE               #'
		print'\t#      client --ip --port  #'
		print '\t############################'


if __name__=='__main__':	
	start = StartApp()
	amountArgs = len(sys.argv)
	if ( start.checkNumberArguments(amountArgs) ):
		if ( start.checkPortIsNumber( sys.argv[4]) ):
			start.initApp()
	else:
		start.help()

