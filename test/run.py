#!/usr/bin/python

import json
from os import listdir
from os.path import isfile, join
from subprocess import Popen, PIPE

inputs = sorted( [ './cases/' + f for f in listdir( './cases' ) if isfile( join( './cases', f ) ) ] )

for x in inputs:
    try:
        p = Popen(['./bin/tester', x], stdout=PIPE)
        output, _ = p.communicate()
        rc = p.returncode

        expected = json.loads( open( x ).read() )
        actual = json.loads( output )

        if not (expected == actual):
            print 'Error: Failed', x
            print 'Expected:', expected
            print 'Actual:', actual
        else:
            print x,'passed.'
    except:
        print 'Error: Failed', x, '- Subprocess failed'
