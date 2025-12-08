key_list = ['name', 'hp', 'mp', 'level']
value_list = ['기사', 200, 30, 5]
character = {}

for key in key_list:
    for value in value_list:
        character[key] = value

print(character)
