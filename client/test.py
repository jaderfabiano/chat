#!/usr/bin/python
import unittest

from chat import StartApp

class TestStart(unittest.TestCase):
	def setUp(self):
		self.start = StartApp()

	def testPortNumber(self):
		self.assertTrue(self.start.checkPortIsNumber(8888))

	def testNotPortNumber(self):
		self.assertFalse(self.start.checkPortIsNumber('port'))

	def testNumberArgumentsWrong(self):
		self.assertFalse(self.start.checkNumberArguments(4))
		
	def testNumberArgumentOks(self):
		self.assertTrue(self.start.checkNumberArguments(5))



if __name__ == "__main__":
    unittest.main()
