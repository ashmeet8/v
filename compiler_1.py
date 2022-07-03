f = open('compiler_1','r')
s = f.read()
s1 = s.split('\n')
instructions = {
        'PUSHBL': '01',
        'PUSHHL': '11',
        'PUSHWL': '21',
        'PUSHDL': '31',
        'PUSHB':  '03',
        'PUSHH':  '13',
        'PUSHW':  '23',
        'PUSHD':  '33',
        'POPB':   '05',
        'POPH':   '15',
        'POPW':   '25',
        'POPD':   '35',
        'PUSHA':  '07',
        'ADD':    '09',
        'SUB':    '89',
        'XOR':    '49',
        'OR':     '69',
        'AND':    '79',
        'SL':     '19',
        'SR':     '39',
        'JUMP'    '0b',
        'BEQ':    '0d',
        'BNE':    '1d',
        'BLT':    '2d',
        'BGE':    '3d',
        'ECALL':  '0f'
}
print(s1)
