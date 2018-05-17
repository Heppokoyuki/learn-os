f = open('hankaku.c', 'w')

f.write('#define X )*2+1\n')
f.write('#define _ )*2\n')
f.write('#define s ((((((((0\n')
f.write('\n')
f.write('char hankaku[4096] = {\n')

i = 1
for line in open('hankaku.txt', 'r'):
    line = line.rstrip()

    if line.startswith('char'):
        f.write('\n/* ' + line + ' */\n')
    elif len(line) == 8:
        line = line.replace('.', '_').replace('*', 'X')
        f.write('s   ' + ' '.join(line))
        if i != 4096:
            f.write('   ,\n')
        else:
            f.write('\n')
        i += 1

f.write('};\n')
f.write('\n')
f.write('#undef X\n')
f.write('#undef _\n')
f.write('#undef s\n')

f.close()
