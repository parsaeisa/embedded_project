NOT_FOUND = '4'
barcode_mapping = {
    '227372240': '1',
    '6262233100610': '2',
    '6269109300020': '3'
}

def decide(barcode):
    best_match = -1
    best_value = NOT_FOUND
    for key, value in barcode_mapping.items():
        if len(key) != len(barcode): continue
        match = 0
        for i in range(len(barcode)):
            if barcode[i] == key[i]: match += 1
        if match > best_match:
            best_match = match
            best_value = value
    return best_value
