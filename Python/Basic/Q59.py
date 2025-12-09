def find_duplicates(list):
    new_list=[]
    for i in list:
        if list.count(i) > 1 and i not in new_list:
            new_list.append(i)
    return new_list

numbers = [1,2,3,2,4,5,3,3,6]

print(find_duplicates(numbers))
#[2, 3]
