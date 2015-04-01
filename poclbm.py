#!/usr/bin/python

from BitcoinMiner import *
from optparse import OptionGroup, OptionParser
from time import sleep
import HttpTransport
import socket

# Socket wrapper to enable socket.TCP_NODELAY and KEEPALIVE
realsocket = socket.socket
def socketwrap(family=socket.AF_INET, type=socket.SOCK_STREAM, proto=0):
	sockobj = realsocket(family, type, proto)
	sockobj.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
	sockobj.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
	return sockobj
socket.socket = socketwrap

VERSION = '20110709'

usage = "usage: %prog [OPTION]... SERVER[#tag]...\nSERVER is one or more [http[s]://]user:pass@host:port          (required)\n[#tag] is a per SERVER user friendly name displayed in stats   (optional)"
parser = OptionParser(version=VERSION, usage=usage)
parser.add_option('--verbose',        dest='verbose',    action='store_true', help='verbose output, suitable for redirection to log file')
parser.add_option('-q', '--quiet',    dest='quiet',      action='store_true', help='suppress all output except hash rate display')

group = OptionGroup(parser, "Miner Options")
group.add_option('-r', '--rate',      dest='rate',       default=1,           help='hash rate display interval in seconds, default=1 (60 with --verbose)', type='float')
group.add_option('-e', '--estimate',  dest='estimate',   default=900,         help='estimated rate time window in seconds, default 900 (15 minutes)', type='int')
group.add_option('-a', '--askrate',   dest='askrate',    default=5,           help='how many seconds between getwork requests, default 5, max 10', type='int')
group.add_option('-t', '--tolerance', dest='tolerance',  default=2,           help='use fallback pool only after N consecutive connection errors, default 2', type='int')
group.add_option('-b', '--failback',  dest='failback',   default=10,          help='attempt to fail back to the primary pool every N getworks, default 10', type='int')
parser.add_option('--no-server-failbacks', dest='nsf',   action='store_true', help='disable using failback hosts provided by server')
parser.add_option_group(group)

group = OptionGroup(parser, "Kernel Options")
#group.add_option('-w', '--worksize', dest='worksize',   default=-1,          help='work group size, default is maximum returned by opencl', type='int')
group.add_option('-w','--worksize',dest='worksize',default=16384,help='number of iterations per kernel execution, default (16384) is whatever worked optimally on my computer',type='int')
#group.add_option('-f', '--frames',   dest='frames',     default=30,          help='will try to bring single kernel execution to 1/frames seconds, default=30, increase this for less desktop lag', type='int')
group.add_option('-f', '--frames',   dest='frames',     default=30,          help='guess of rate of kernel executions (Hz), default=30, affects rate of asking for new work', type='int')
group.add_option('-s', '--sleep',    dest='frameSleep', default=0,           help='sleep per frame in seconds, default 0', type='float')
parser.add_option_group(group)

(options, options.servers) = parser.parse_args()

miner = None
#miner2= None
try:
	miner = BitcoinMiner(options, VERSION, HttpTransport.HttpTransport)
	miner.start()
#	miner2 = BitcoinMiner(options, VERSION, HttpTransport.HttpTransport)
#	miner2.start()
except KeyboardInterrupt:
	print '\nbye'
finally:
	if miner: miner.stop()
#	if miner2: miner2.stop()
sleep(1.1)
