key_list = ['name', 'hp', 'mp', 'level']
value_list = ['기사', 200, 30, 5]
character = {}

for i in range(len(key_list)):
    for j in range(len(value_list)):
        if i == j:           
            character[key_list[i]] = value_list[j]

print(character)

