'''

Copyright 2022 FDL(Future cryptograph Design Laboratory, Kookmin University

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

'''
###################################################
'''
    Binary Separable Goppa Code used in PALOMA
    developed by FDL/KMU
'''
###################################################

'''
    F2m = GF(2^13) (i.e., m = 13)
    Separable Goppa Polymoial g(X) with degree t in F2m[X] (t-error collectable code)

    n + t <= q^m = 2^13 = 8192
    k >= n - mt = n - 13t

    parameters:
    PALOMA128: n = 3904(61), k = 3072, n-k = 832(13), m = 13, t = 64
    PALOMA192: n = 5568(87), k = 3904, n-k = 1664(26), m = 13, t = 128
    PALOMA256: n = 6592(103), k = 4928, n-k = 1664(26), m = 13, t = 128

    Toy parameters:
    n = 37, k = 19, n-k = 18, t = 3, m = 6, f = z^6 + z^4 + z^3 + z + 1
    n = 100, k = 72, n-k = 28, t = 4, m = 7, f = z^7 + z + 1
    n = 120, k = 64, n-k = 56, t = 8, m = 7, f = z^7 + z + 1
    n = 241, k = 121, n-k = 120, t = 15, m = 8, f = z^8 + z^4 + z^3 + z^2 + 1

    n = 53, k = 27, n-k = 26, t = 2, m = 13, f = z^13 + z^7 + z^6 + z^5 + z^0
    n = 79, k = 40, n-k = 39, t = 3, m = 13, f = z^13 + z^7 + z^6 + z^5 + z^0

'''
###################################################

reset()
var('z')

###################################################

def line():
    print ("\n====================================================")

def newline():
    print (" ")

line()

###################################################
# parameters: n, t, m, irr_poly
###################################################
paloma_param = [
    [37, 3, 6, z^6 + z^4 + z^3 + z + 1],
    [100, 4, 7, z^7 + z + 1],
    [120, 8, 7, z^7 + z + 1],
    [241, 15, 8, z^8 + z^4 + z^3 + z^2 + 1],

    [53, 2, 13, z^13 + z^7 + z^6 + z^5 + z^0],
    [79, 3, 13, z^13 + z^7 + z^6 + z^5 + z^0],

    [216, 8, 13, z^13 + z^7 + z^6 + z^5 + 1],
    [424, 16, 13, z^13 + z^7 + z^6 + z^5 + 1],

    [3904, 64, 13, z^13 + z^7 + z^6 + z^5 + 1],
    [5568, 128, 13, z^13 + z^7 + z^6 + z^5 + 1],
    [6592, 128, 13, z^13 + z^7 + z^6 + z^5 + 1],
]

n, t, m, f = paloma_param[8]
k = n - m*t

###################################################

R2.<z> = GF(2)[]
F2m.<z> = GF(2^m, modulus = R2(f))
R2m.<X> = PolynomialRing(F2m)

###################################################
# function for hex representation 
###################################################

def str_f2m_hex(x):
    return "0x{:04x}".format(ZZ(list(F2m(x).polynomial()), base = 2))
#    return hex(ZZ(list(F2m(x).polynomial()), base = 2))

def show_mat_hex(m):
    nrows, ncols = m.nrows(), m.ncols()
    for r in range(0, nrows):
        str = "[ "
        for c in range(0, ncols):
            str += str_f2m_hex(m[r][c]) + " "
        print (str, "]")

def show_poly_hex(f):
    show_mat_hex(matrix(list(f)))

###################################################
# Generate Random Binary Separable Goppa Code
###################################################

print ("Random Binary Separable Goppa Code")
print ("n = {}({}), n-k = {}({}), t = {}, m = {}".format(n, n/64, n-k, (n-k)/64, t, m))
newline()

###################################################

listF2m = list(F2m)
mbitset = list(range(0,2^m,1))

###################################################
# Generate Support Set L and Separable Goppa polynomial g(X)
###################################################

# shuffle(mbitset)

'''
    Support set L
'''
L = [listF2m[j] for j in mbitset[:n]]
print ("Support Set L")
print (L)
show_mat_hex(matrix(L))
line()

'''
    Separable Goppa polynomial g(X)
'''
g = prod([(X+listF2m[j]) for j in mbitset[n:n+t]])
print ("Goppa Poly. g(X)")
print (g)
show_poly_hex(g)
print("roots = ", [listF2m[j] for j in mbitset[n:n+t]])
line()

###################################################
# Compute Parity-check Matrix H = A*B*C
###################################################

'''
    Matrix A
'''
coeffg = list(g) + [0]*(t-1)
A = matrix([coeffg[i:i+t] for i in [1..t]])

print ("\nA")
#print (A)
show_mat_hex(A)
newline()


'''
    Matrix B*C
'''
time B = matrix(F2m, t, n, lambda r, c: (L[c]^r))
print ("Parity-check Matrix H = B")
#print (B)
show_mat_hex(B)
newline()

#T1 = [g(L[c]) for c in range(0,n)]
#T2 = [g(L[c])^-1 for c in range(0,n)]
#print("T1: ", T1)
#print("T2: ", T2)

time BC = matrix(F2m, t, n, lambda r, c: (L[c]^r) * (g(L[c])^-1) )

print ("\nParity-check Matrix H = BC")
show_mat_hex(BC)
#print (BC)
newline()

time H = A*BC
print ("\nParity-check Matrix H = ABC")
#print (H)
show_mat_hex(H)
newline()

'''
    Parity-check matrix derived from (X-aj)^-1
'''
'''
H1 = []
for i in [0..n-1]:
    inv = R2m((g - g(L[i]))/(X-L[i]))*g(L[i])^-1
    H1 += [list(inv)]

H1 = Matrix(F2m, H1).transpose()
print("H1 == H?", H1 == H)
'''

###################################################
# Modified Patterson Decoding for Binary Separable Goppa Code
###################################################

'''
    Given f s.t gcd(f,g),
    find f^-1 such that f^-1*f = 1 (mod g)
'''
def getInv(f, g):
    t = g.degree()
    d0, d1 = R2m(f), R2m(g)
    a0, a1 = R2m(1), R2m(0)

    while d1 != 0:
        r = d0%d1
        q = R2m((d0 - r)/d1)
        d0, d1 = d1, r
        a2 = a0 - q*a1
        a0, a1 = a1, a2

    return a0*d0.leading_coefficient()^-1

###################################################

'''
    Find a2, b1 such that b1*s_hat = a2 (mod g12) with deg condition
'''
def EEA_for_keyeqn(s_hat, g12, dega, degb):
    a0, a1 = R2m(s_hat), R2m(g12)
    b0, b1 = R2m(1), R2m(0)

    while a1 != 0:
        q, r = a0.quo_rem(a1)
        a0, a1 = a1, r
        b2 = b0 - q*b1
        b0, b1 = b1, b2
        if a0.degree() <= dega and b0.degree() <= degb:
            break
    return a0, b0

###################################################

'''
    Compute Square Root of f(X) mod g12(X)
'''

def get_sqrt(f, g):
    sqrtx = power_mod(R2m(X), 2^(m*t-1), g)
    print("sqrtx^2%g == X?", sqrtx^2%g == X)
    print("sqrt(X) mod g12 =", sqrtx)
    degf = R2m(f).degree()
    listf = list(f)
    fe = [sqrt(listf[2*j]) for j in [0..floor(degf/2)]]
    fo = [sqrt(listf[2*j+1]) for j in [0..floor((degf-1)/2)]]

    sqrtf = (R2m(fe) + R2m(fo)*sqrtx)%g
    return sqrtf

###################################################

'''
    Given f, find a(X), b(X) such that f = a^2(X) + b^2(X)*X
'''
def get_a2b2x(f):
    degf = R2m(f).degree()
    listf = list(f)
    fe = [sqrt(listf[2*j]) for j in [0..floor(degf/2)]]
    fo = [sqrt(listf[2*j+1]) for j in [0..floor((degf-1)/2)]]
    a = R2m(fe)
    b = R2m(fo)
    return a, b

###################################################

line()

###################################################
# Step 0. Generate Random Error Vector with Hamming Weight t
###################################################

nset = list(range(0,n))
shuffle(nset)

e = [0]*n
for i in nset[0:t]:
    e[i] = 1
#print ("Error vector e\n", e)
print ("Error Polynomial e(X) =", R2m(e))
line()

'''
    error locator polynomial sigma_t = a_t^2 + b_t^2*X for checking correctness
'''
sigma_t = R2m(1)
for i in range(0,n):
    if e[i] == 1:
        sigma_t = sigma_t * (X + L[i])

a_t, b_t = get_a2b2x(sigma_t)

print("sigma_t(X) =", sigma_t)
print("a_t(X) =", a_t)
print("b_t(X) =", b_t)
print("R2m(a_t^2 + b_t^2*X) == sigma_t?", R2m(a_t^2 + b_t^2*X) == sigma_t)
newline()

###################################################
# Step 1. Compute Syndrome s(X) of e(X)
###################################################

He = H * vector(e)
s = R2m(list(He))

'''
H1e = H1 * vector(e)
print("He == H1e?", He == H1e)
'''

print("s(X)     =", s)

'''
    Checking Correctness
'''
syndrome = R2m(0)
for i in [0..n-1]:
    syndrome += e[i]*R2m((g - g(L[i]))/(X-L[i]))*g(L[i])^-1
print("syndrome =", syndrome)
print("s(X) == syndrome?", s == syndrome)

###################################################
# Step 2. Find Error Locator Polynomial sigma(X)
###################################################

'''
    Checking Correctness
'''
print("sigma_t*s%g == sigma_t.derivative()?", sigma_t*s%g == sigma_t.derivative())
newline()

###################################################

'''
    Derive Key Equation
'''
s_ast = R2m(1) + X*s(X)
g1 = gcd(g, s)
g2 = gcd(g, s_ast)
g12 = R2m(g/g1/g2)
s2_ast = R2m(s_ast/g2)
s1 = R2m(s/g1)

u = (g1 * s2_ast * getInv(g2*s1, g12))%g12
print("g2*s1*getInv(g2*s1, g12)%g12 == 1?", g2*s1*getInv(g2*s1, g12)%g12 == 1)

s_hat = get_sqrt(R2m(u), R2m(g12))

print("s_hat^2%g12 == u?", s_hat^2%g12 == u)

'''
    Solve Key Equation
'''

a2, b1 = EEA_for_keyeqn(s_hat, g12, floor(t/2)-g2.degree(), floor((t-1)/2)-g1.degree())
print ("b1*s_hat%g12 == a2?", b1*s_hat%g12 == a2)

'''
    Compute a, b
'''

a = a2*g2
b = b1*g1
print("a(X) =", a)
print("b(X) =", b)

'''
    Checking Correctness
'''
print ("b^2*s_ast%g == a^2*s%g?", b^2*s_ast%g == a^2*s%g)
print ("b^2*(1+X*s)%g == a^2*s%g?", b^2*(1+X*s)%g == a^2*s%g)


sigma = (a^2 + b^2*X).monic()
print ("sigma == sigma_t?", sigma == sigma_t)

###################################################
# Step 3. Find Roots of sigma(X)
###################################################


err_support_set = []
for i in [0..n-1]:
    if sigma(L[i]) == 0:
        err_support_set += [i]
print("recovered supp(e) =", err_support_set)
line()

###################################################
'''
    Result
'''

print ("\nDo we find the correct error?", err_support_set == R2m(e).exponents())
line()