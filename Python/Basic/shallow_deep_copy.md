>>> Alist

[1, 2, 3]

>>> Blist

[1, 2, 3]

>>> Blist[0] = 100

>>> Blist

[100, 2, 3]

>>> Alist

[100, 2, 3]

>>> Clist = list(Alist)

>>> Clist

[100, 2, 3]

>>> Alist

[100, 2, 3]

>>> Alist[0] = 200

>>> Alist

[200, 2, 3]

>>> Blist

[200, 2, 3]

>>> Clist

[100, 2, 3]

>>> import copy

>>> Clist = copy.deepcopy(Alist)

>

