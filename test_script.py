 #
 # @project IFJ2014
 # @team	Dusan Drevicky 		<xdrevi00@stud.fit.vutbr.cz>
 # 			Michal Gabonay		<xgabon00@stud.fit.vutbr.cz>
 #			Denis Gerguri		<xgergu01@stud.fit.vutbr.cz>
 #			Vladislav Zavada	<xzavad12@stud.fit.vutbr.cz>
 #			Martin Kocour 		<xkocou08@stud.fit.vutbr.cz>
 #
 

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
import os
import re

TCodes = {'OK' : 0, 'ERR_LEX' : 1, 'ERR_SYNTAX' : 2, 'ERR_SEM_DEF' : 3, 'ERR_SEM_TYPE' : 4,
          'ERR_SEM_OTHER' : 5, 'ERR_RUN_READ_NUMBER' : 6, 'ERR_RUN_UNINITIALIZED' : 7,
          'ERR_RUN_DIV_ZERO' : 8, 'ERR_RUN_OTHER' : 9, 'ERR_INTERNAL' : 99}

TCodes_inv = {0 : 'OK', 1 : 'ERR_LEX', 2 : 'ERR_SYNTAX', 3 : 'ERR_SEM_DEF', 4 : 'ERR_SEM_TYPE',
              5 : 'ERR_SEM_OTHER', 6 : 'ERR_RUN_READ_NUMBER', 7 : 'ERR_RUN_UNINITIALIZED', 
              8 : 'ERR_RUN_DIV_ZERO', 9 : 'ERR_RUN_OTHER', 99 : 'ERR_INTERNAL'}


file_list = os.listdir('testy')
files = []
for i in file_list:
   files.append(i.split('-'))

files = sorted(files, key=lambda f: int(f[0]))
for i, val in enumerate(files):
   files[i] = '-'.join(val)

# files obsahuje zoradene nazvy suborov

re = re.compile(r'^([0-9]*)-([^\.]*)\.p')

results =[]

for file_name in files:
   result =  re.match(file_name)
   print result.group(0)
   #print result.group(1), TCodes[result.group(2)]
   return_value = os.system('./main testy/' + file_name) >> 8
   if (return_value == TCodes[result.group(2)]):
     results.append('Test ' + result.group(1) + ': SUCCESS    Expected: ' + result.group(2) + "    Got: " + TCodes_inv[return_value])
   else:
      results.append('Test ' + result.group(1) + ': FAIL    Expected: ' + result.group(2) + "    Got: " + TCodes_inv[return_value])

for line in results:
   print line

