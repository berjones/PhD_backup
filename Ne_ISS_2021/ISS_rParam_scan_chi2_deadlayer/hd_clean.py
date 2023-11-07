import os
import sys
import re 

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# This script takes a hdtv file and cleans it to a nice output, appending the output
# file with -clean. Give file as a command line argumennt:
# e.g. python3 hd_clean.py hdtv_out.txt
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# input the hdtv file to be cleaned 
input_file = sys.argv[1]				#open file given as command line argument


def convert_scientific_notation(number):
    match = re.match(r"(.+)\((\d+)\)e([+-]\d+)", number)
    if match:
        num = match.group(1)
        err = int(match.group(2))
        exp = int(match.group(3))
        if '.' in num:
            num_str = str(num)
            decimal_places = len(num_str.split('.')[1])
            adjusted_err = int(err * 10 ** (exp - decimal_places))
            adjusted_num = int(float(num) * 10 ** exp)
            return f"{adjusted_num}({adjusted_err})"
        else:
            adjusted_num = int(num) * 10 ** exp
            return f"{adjusted_num}({err * 10 ** exp})"
    else:
        return number



def extract_workfit_data(input_file, output_file):
    with open(input_file, 'r') as file:
        lines = file.readlines()

    workfit_data = []
    current_workfit = None

    for line in lines:
        if line.startswith('WorkFit on spectrum:'):
            if current_workfit:
                workfit_data.append(current_workfit)

            workfit_name = line.split('(')[-1].strip(')\n')
            current_workfit = {'name': workfit_name}

        elif current_workfit and line.strip() != '':
            columns = line.split()
            if len(columns) == 7:
                pos = columns[4]
                vol = columns[5]
                width = columns[6]
                current_workfit['pos'] = convert_scientific_notation(pos)
                current_workfit['vol'] = convert_scientific_notation(vol)
                current_workfit['width'] = convert_scientific_notation(width)

    if current_workfit:
        workfit_data.append(current_workfit)

    with open(output_file, 'w') as file:
        file.write('projection,pos,vol,width\n')
        for workfit in workfit_data:
            file.write(f"{workfit['name']},{workfit['pos']},{workfit['vol']},{workfit['width']}\n")

    print("Data extraction completed.")

output_file = os.path.splitext(input_file)[0] + '-clean' + os.path.splitext(input_file)[1]

extract_workfit_data(input_file, output_file)