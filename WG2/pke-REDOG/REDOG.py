import ctypes
from ctypes import *
import platform
from sage.all import *

from sage.matrix.constructor import matrix, diagonal_matrix
from sage.matrix.matrix import is_Matrix
from sage.structure.element import is_Vector
from sage.sets.set import Set
from sage.matrix.constructor import column_matrix
from sage.rings.finite_rings.finite_field_constructor import GF
from sage.categories.cartesian_product import cartesian_product
from sage.modules.free_module_element import vector
from sage.modules.free_module import VectorSpace
from sage.rings.integer import Integer
from sage.misc.cachefunc import cached_method
from sage.rings.integer_ring import ZZ
from sage.functions.other import floor
from sage.coding.linear_code import AbstractLinearCode
from sage.coding.relative_finite_field_extension import *
from sage.structure.sage_object import SageObject
from sage.functions.log import *
from sage.rings.polynomial.skew_polynomial_element import *
from sage.coding.encoder import Encoder
from sage.coding.decoder import Decoder, DecodingError
from sage.coding.relative_finite_field_extension import RelativeFiniteFieldExtension
import warnings
import random
warnings.filterwarnings('ignore')


if 'Windows' == platform.system() :
    path = './x64/Debug/Dll1.dll'
    c_module = ctypes.windll.LoadLibrary(path)
elif 'Linux' == platform.system() :     # 리눅스 운영체제에서 c 모듈 로드
    path = "/home/hd/kpqclean-Round1/pke-REDOG/REDOG.so"
    c_module = ctypes.cdll.LoadLibrary(path)
else :
    raise OSError()

inputFunc = c_module.input
inputFunc.argtypes = [ctypes.POINTER(ctypes.c_int)]
inputFunc.restype = None
a = ctypes.c_int()
inputFunc(a)
#print(a.value)


if(a.value == 128):
    n = 44
    k = 8
    l = 37
    q = 2
    m = 83
    r = 18 
    lambda_sp = 3
    t = 6 

elif(a.value == 192):
    n = 58
    k = 10
    l = 49
    q = 2
    m = 109
    r = 24
    lambda_sp = 3
    t = 8


elif(a.value == 256):
    n = 72
    k = 12
    l = 61
    q = 2
    m = 135
    r = 30
    lambda_sp = 3
    t = 10

#print(n)




class GabidulinCode(AbstractLinearCode):
    _registered_encoders = {}
    _registered_decoders = {}
    def __init__(self, ground_field, extension_field, linearized_poly_ring, length, dimension):
        F  = ground_field 
        Fm = extension_field 
        FE = RelativeFiniteFieldExtension(Fm, F) 
        m = FE.extension_degree() 

        if not length <= m or length not in ZZ or length < 1:
            raise ValueError("length of the code must be a positive integer less than or equal to the extension degree (m) = %d" % m )
        if not dimension <= length or dimension not in ZZ or dimension < 1:
            raise ValueError("dimension of the code must be a positive integer less than or equal to the length of the code (n) = %d" % length )
        super(GabidulinCode, self).__init__(Fm, length, "EvaluationEncoder", "GabidulinGao")
        self._dimension = dimension
        self._length = length #n
        self._ground_field = F
        self._extension_field = Fm
        self._extension_degree = m #m
        self._relative_extension = FE
        self._linearized_poly_ring = linearized_poly_ring
        self._Frob = linearized_poly_ring.twist_map()
        
    def length(self):
        return self._length
    
    def dimension(self):
        return self._dimension
    
    def minimum_distance(self):
        n = self._length
        k = self._dimension
        return (n-k+1)
    
    def linearized_poly_ring(self):
        
        return self._linearized_poly_ring
        
    def cardinality(self):
        m = self._extension_degree
        k = self._dimension
        q = self._ground_field.order()
        K = q**(m*k)
        return K
    
    def ground_field(self):
        return self._ground_field
    
    def extension_field(self):
        return self._extension_field
    
    def extension_degree(self):
        return self._extension_degree
    
    def rank_weight(self,codeword_in_vector_repr):
        codeword_in_matrix_repr = self.map_into_ground_field(codeword_in_vector_repr)
        rank_weight = codeword_in_matrix_repr.rank()
        return rank_weight
    
    def rank_distance(self,codeword_in_vector_repr1,codeword_in_vector_repr2):
        codeword_in_matrix_repr1 = self.map_into_ground_field(codeword_in_vector_repr1)
        codeword_in_matrix_repr2 = self.map_into_ground_field(codeword_in_vector_repr2)
        rank_distance = (codeword_in_matrix_repr1-codeword_in_matrix_repr2).rank()
        return rank_distance
    
    def polynomial_basis(self):
        m = self._extension_degree
        FE = self._relative_extension
        absolute_field_basis = FE.absolute_field_basis()
        basis = absolute_field_basis[0:m]
        return vector(basis)
    
    def normal_basis(self):
        m = self._extension_degree
        F = self._ground_field
        Fm = self._extension_field
        V = VectorSpace(F,m)
        f=F.polynomial()
        alpha = f.any_root(Fm)
        g=f/((Fm.gen()-alpha)*f.derivative()(alpha))
        u=Fm.random_element()
        normal_element= g(u)
        normal_basis = [normal_element]
        for i in range(1,m):
            normal_basis.append(self.q_power(normal_element,i))
        basis_ground = self.map_into_ground_field(normal_basis)	
        while matrix(basis_ground).is_singular():
            u=Fm.random_element()
            normal_element= g(u)
            normal_basis = [normal_element]
            for i in range(1,m):
                normal_basis.append(self.q_power(normal_element,i))
                basis_ground = self.map_into_ground_field(normal_basis)
        self._is_normal_basis(normal_basis)
        return vector(normal_basis)
    
    def _is_normal_basis(self,normal_basis):
        length = len(normal_basis)
        for i in range(length):
            if self.q_power(normal_basis[0],i)!=normal_basis[i]:
                raise ValueError("value of 'basis' keyword is not a normal basis")
                
    def _trace(self,alpha):
        m = self._extension_degree
        trace = 0
        for i in range(m):
            trace = trace + self.q_power(alpha,i)
        return trace
    
    def dual_basis(self , basis):
        m = self._extension_degree
        F = self._ground_field
        Fm = self._extension_field
        basis = vector(basis)
        entries = [self._trace(bi * bj) for bi in basis for bj in basis]
        B = matrix(Fm, m, entries).inverse()
        dual_basis = []
        dual_basis = [sum(x * y for x, y in zip(col, basis))
                  for col in B.columns()]
        self._is_dual_basis(basis, dual_basis)
        return vector(dual_basis)
    
    
    def _is_dual_basis(self, basis, dual_basis):
        
        m = self._extension_degree
        entries = [self._trace(betai * gammaj) for betai in basis for gammaj in dual_basis]
        I = matrix(m, entries)
        if I!=matrix.identity(m):
            raise ValueError("value of 'basis' keyword and 'dual_basis' keyword are not dual")
            
    def _normal_dual_basis_matrix(self,normal_basis):    
        n = len(normal_basis)
        dual_basis = self.dual_basis(normal_basis)
        B = matrix(n, n, lambda i, j: normal_basis[(i + j) % n])
        B_dual = matrix(n, n, lambda i, j: dual_basis[(i + j) % n])
        return B,B_dual
    
    def evaluation_points(self,basis=None):
        n = self.length()
        if basis is None:
            basis = self.polynomial_basis()
        evaluation_points=[]
        for i in range(n):
            evaluation_points.append(basis[i])
        return vector(evaluation_points)
    
    def map_into_ground_field(self,vector):
        FE = self._relative_extension
        vector_length = len(vector)
        for i in range(vector_length):
            if vector[i] not in self.extension_field():
                        raise ValueError("input should be an element of %s" % self.extension_field() )
        vector_ground = []
        for i in range(vector_length):
                 vector_ground.append(FE.relative_field_representation(vector[i]))
        return matrix(vector_ground)
    
    def map_into_extension_field(self,matrix):
        m = self._extension_degree
        FE = self._relative_extension
        F = self._ground_field
        V = VectorSpace(F,m)
        rows = matrix.nrows()	#len(matrix)
        for i in range(rows):
            if matrix[i] not in V:
                        raise ValueError("input should be an element of %s" % self.ground_field() )
        matrix_extension = []
        for i in range(rows):
                 matrix_extension.append(FE.absolute_field_representation(matrix[i]))
        return matrix_extension
    
    
    def linear_independency_over_ground_field(self,basis):
        F = self.ground_field()
        m = self._extension_degree
        V = VectorSpace(F, m)
        basis_ground = self.map_into_ground_field(basis)
        if V.linear_dependence(basis_ground):
                raise ValueError("The elements provided are not linearly independent over %s" % self.ground_field())	
                
                
    
    def frobenius_automorphism(self):
        return self._Frob
    
    def random_linearized_poly(self,dual_code = None):
        k = self._dimension
        n = self._length
        linearized_poly_ring = self._linearized_poly_ring
        if dual_code:
            linearized_poly = linearized_poly_ring.random_element(degree=(0,n-k-1)) 
        else:
            linearized_poly = linearized_poly_ring.random_element(degree=(0,k-1))  
        return linearized_poly
    
    def _right_LEEA(self, a, b, d_stop):
        n = self.length()
        k = self.dimension()
        L = self._linearized_poly_ring
        r_last = a
        r = b
        u_last = L.zero()
        u = L.one()
        v_last = L.one()
        v = L.zero()
        while(r.degree() >= d_stop):
            (q , R) = r_last.right_quo_rem(r)
            (u , u_last) = (u_last - q * u , u)
            (v , v_last) = (v_last - q * v , v)
            (r , r_last) = (R,r)
        return r ,u ,v
    
    
    
    def _lagrange_interpolating_polynomial(self, polynomial_coefficients, basis=None):
        n = self.length()
        m = self._extension_degree
        k = self.dimension()
        L = self._linearized_poly_ring
        if basis is None:
            basis = self.polynomial_basis()
        evaluation_points = self.evaluation_points(basis)

        if not type(polynomial_coefficients)==list:
                coefficients = polynomial_coefficients.list()
        try:
            if not self._is_normal_basis(basis) and n.divides(m):     
                    polynomial = L(coefficients)
                    lagrange_interpolating_polynomial = L(self.evaluate_linearized_poly(polynomial,evaluation_points).list())	        
        except ValueError:
            pairs = list(zip(evaluation_points, polynomial_coefficients));
            lagrange_interpolating_polynomial = L.lagrange_polynomial(pairs)

        return lagrange_interpolating_polynomial
    
    def evaluate_linearized_poly(self,linearized_poly=None,evaluation_points=None):
        k = self._dimension
        linearized_poly_ring = self._linearized_poly_ring
        if linearized_poly is None:
            linearized_poly = self.random_linearized_poly(linearized_poly_ring)
        if evaluation_points is None:
            evaluation_points = self.evaluation_points() 
        evaluation = linearized_poly.multi_point_evaluation(evaluation_points)
        return vector(evaluation)
    
    
    def q_power(self,field_element,exponent):
        F = self._ground_field
        Fm = self._extension_field
        q = F.order()
        Fm_degree = Fm.degree()
        result = field_element**(q**(exponent % Fm_degree))

        return result
    
    def code_space(self):
        return VectorSpace(self.extension_field(),self.length())
        
    def _create_generator_matrix(self,evaluation_points,k):
        n = self._length
        Fm = self._extension_field 
        G = matrix(Fm, k, n, lambda i,j: self.q_power(evaluation_points[j] , i))
        return G
        
    def generator_matrix(self,basis=None):
        Frob = self._Frob
        k = self.dimension()
        n = self.length()
        m = self.extension_degree()
        Fm = self.extension_field()
        if basis is None:
            basis = self.polynomial_basis()
        if not len(basis) == m :
                    raise ValueError("length of the basis must be %d" %  m )
        try:
            if not self._is_normal_basis(basis):    
                    G=self._normal_dual_basis_matrix(basis)[1][:,0:k].transpose()        
        except ValueError:
            evaluation_points = self.evaluation_points(basis)  
            G = self._create_generator_matrix(evaluation_points,k)  

            #G.set_immutable()
        return G
        
    def parity_check_matrix(self,basis=None):
        Frob = self._Frob
        k = self.dimension()
        n = self.length()
        m = self.extension_degree()
        Fm = self.extension_field()
        F = self.ground_field()
        q=F.order()
        if basis is None:
            basis = self.polynomial_basis()
        if not len(basis) == m :
            raise ValueError("length of the basis must be %d" %  m )
        try:
            if not self._is_normal_basis(basis):   
                        H=self._normal_dual_basis_matrix(basis)[0][k:,:]        
        except ValueError:      
            evaluation_points = self.evaluation_points(basis)#here there was no basis between paranthesis
            coefficient_matrix = matrix(Fm, n - 1, n, lambda i,j: self.q_power(evaluation_points[j] , -n + k + 1 + i))
            solution_space = coefficient_matrix.right_kernel()
            parity_matrix_space = solution_space.basis() 
            parity_points =  vector(parity_matrix_space[0])
            H = self._create_generator_matrix(parity_points,n-k)    
            #H.set_immutable()
        return H
    
    def dual_code(self):
        F = self._ground_field
        Fm = self._extension_field
        k = self._dimension
        n = self._length
        L = self._linearized_poly_ring
        C_dual = GabidulinCode(F,Fm,L,n,n-k)
        return C_dual
    
    def _repr_(self):
        return "Gabidulin Code Gab[%s,%s] over %s" % (self.length(), self.dimension(), self.extension_field())

    def _latex_(self):
        return "\\textnormal{Gabidulin Code Gab}[%s,%s] \\textnormal{over }%s" % (self.length(), self.dimension(), self.extension_field())

    def __eq__(self, other):
        return isinstance(other, GabidulinCode)\
                and self.length() == other.length()\
                and self.dimension() == other.dimension()\
                and self.ground_field() == other.ground_field()\
                and self.extension_field() == other.extension_field()
                
                
class GabidulinCodePolynomialEvaluationEncoder(Encoder):
    def __init__(self, code):
            super(GabidulinCodePolynomialEvaluationEncoder, self).__init__(code)

    def __eq__(self, other):
        return isinstance(other, GabidulinCodePolynomialEvaluationEncoder)\
                    and self.code() == other.code()
        def _repr_(self):
            return "Polynomial evaluation based encoder for %s" % self.code()
    def _latex_(self):
            return "\\textnormal{Polynomial evaluation based encoder for }%s" % self.code()._latex_()
        
        
    def _is_codeword(self,codeword,basis=None):
        C = self.code()
        H = C.parity_check_matrix(basis)
        Code = H.right_kernel()
        if codeword in Code:
            return True
        else:
            return False
        
    def encode(self,linearized_poly,basis=None):   
        C = self.code()
        k = C.dimension()
        degree = linearized_poly.degree()
        if basis is None:
            basis = C.polynomial_basis()
        try:
            if not C._is_normal_basis(basis):  
                    dual_basis=C.dual_basis(basis)  
                    evaluation_points = C.evaluation_points(dual_basis)     
        except ValueError:
            evaluation_points = C.evaluation_points(basis)
            
        if not degree < k:
            raise ValueError("degree of linearized polynomial must be less than k = %d" %  k )
        codeword = C.evaluate_linearized_poly(linearized_poly,evaluation_points)
        return vector(codeword)
    
    def unencode_nocheck(self, codeword, basis=None):

        C = self.code()
        L = self.message_space()
        if basis is None:
            basis = C.polynomial_basis()
        p = C._lagrange_interpolating_polynomial(codeword,basis)
        return p
    
    def message_space(self):
        C = self.code()
        L = C.linearized_poly_ring()
        return L


class GabidulinCodeGaoDecoder(Decoder):
    def __init__(self, code):
        if not isinstance(code, GabidulinCode):
            raise ValueError("The code given as input is not a Gabidulin code")
        super(GabidulinCodeGaoDecoder, self).__init__(code, code.code_space(),"EvaluationEncoder")
    def _repr_(self):

        return "Gao-like decoder for %s" % self.code()


    def _latex_(self):
        return "\\textnormal{Gao-like decoder for }%s" % self.code()._latex_()


    def __eq__(self, other):
        return (isinstance(other, GabidulinCodeGaoDecoder)
            and self.code() == other.code())
    
    def _minimal_subspace_polynomial(self,basis=None):
        C = self.code()
        E = self.connected_encoder()
        n, m = C.length(), C.extension_degree()
        L = E.message_space()
        x = L.gen()
        if basis is None:
            basis = C.polynomial_basis()
        evaluation_points = C.evaluation_points(basis)

        if n==m:
            M = x**m-L.one()
        else:
            try:
                if not C._is_normal_basis(basis) and n.divides(m):
                    M = x**n-L.one()
            except ValueError:
                M = L.minimal_vanishing_polynomial(evaluation_points)

        return M
    
    def decode_to_code(self, received_word, basis=None):
        C = self.code()
        E = self.connected_encoder()
        n, k = C.length(), C.dimension()
        L = E.message_space()

        if received_word not in C.code_space():
            raise ValueError("The word does not belong to the code-space")

        if basis is None:
            basis = C.polynomial_basis()

        if E._is_codeword(received_word,basis):
            estimated_codeword = received_word
            return estimated_codeword	

        evaluation_points = C.evaluation_points(basis)
        lagrange_interpolating_polynomial = C._lagrange_interpolating_polynomial(received_word,basis)
        M = self._minimal_subspace_polynomial(basis)
        d_stop = (k + n) // 2    
        (r_out, u_out, v_out) = C._right_LEEA(M, lagrange_interpolating_polynomial,d_stop)
        (estimated_message, r) = r_out.left_quo_rem(u_out)
        estimated_codeword = E.encode(estimated_message, basis)
        error_vec = make_random_error(q, m, k, (2*k-n))
        if not r.is_zero():
            estimated_codeword_1 = received_word - vector(error_vec)
            return estimated_codeword_1
        #    raise DecodingError("Decoding failure, as the number of corrupted positions is larger than floor({d-1}/{2}) = %d of the %s"\
        #        % (self.decoding_radius(),C)) 

        if estimated_message not in L:
            estimated_codeword_1 = received_word - vector(error_vec)
            return estimated_codeword_1
        #    raise DecodingError("Decoding failure, because the decoded message is not a valid message")

        if not E._is_codeword(estimated_codeword,basis):
            estimated_codeword_1 = received_word - vector(error_vec)
            return estimated_codeword_1
        #    raise DecodingError("Decoding failed because the decoded word is not a codeword of the code")

        return estimated_codeword
    
    def decode_to_message(self, received_word, basis=None):
        C = self.code()
        E = self.connected_encoder()
        if basis is None:
            basis = C.polynomial_basis()
        codeword = self.decode_to_code(received_word,basis)
        p = E.unencode_nocheck(codeword,basis) 	
        return p
    
    def decoding_radius(self):
        C = self.code()
        t_max = (C.minimum_distance() - 1) // 2
        return t_max    


GabidulinCode._registered_encoders["EvaluationEncoder"] = GabidulinCodePolynomialEvaluationEncoder
GabidulinCode._registered_decoders["GabidulinGao"] = GabidulinCodeGaoDecoder




def Parity_check_function(matrix):
    Field = matrix.base_ring()
    nor = matrix.nrows()
    noc = matrix.ncols()
    echel_mat = matrix.echelon_form()
    ef = zero_matrix(Field, noc-nor, noc)
    for i in range(noc-nor):
        for j in range(noc):
            if j <= nor-1:
                ef[i,j] = echel_mat[j, i+nor]
            else:
                if j == nor+i:
                    ef[i,j] = 1
                else:
                    ef[i,j] = 0
    PC_mat = Matrix(Field, noc-nor, noc, ef)
    return PC_mat


def nonsingular_matrix_inv(Fieldbase_q, length_m, length_n, length_k, lambda_sp_val, seed_num = 0):
    Fqm = FiniteField(Fieldbase_q**length_m, 'z') 
    Fq = FiniteField(Fieldbase_q)
    rankS = 0
    diff_n_k = length_n-length_k
    set_random_seed(seed_num)
    while rankS != diff_n_k:
        sq = zero_matrix(Fqm.base_ring(), nrows=length_m, ncols=lambda_sp_val)
        while rank(sq) < lambda_sp_val:
            s =  random_matrix(Fqm, 1, lambda_sp_val)
            for i in range(lambda_sp_val) :
                s[0, i] = s[0, i]/s[0, 0]
            for i in range(length_m):
                for j in range(lambda_sp_val):
                    sq[i, j] = matrix(ZZ(s[0,j].integer_representation() ).digits(base=Fieldbase_q, padto=length_m))[0,i]
        S_inv_1 = identity_matrix(Fqm,diff_n_k)
        for i in range(diff_n_k):
            S_inv_1[i,:] = zero_matrix(Fqm.base_ring(), nrows=1, ncols = diff_n_k)
            for j in range(diff_n_k):
                S_inv_1[i, j] = Fqm.fetch_int(0)
                for k in range(lambda_sp_val):
                    ran_elt = int(Fq.random_element())
                    S_inv_1[i, j] = S_inv_1[i, j]+(Fqm.fetch_int(ran_elt))*s[0, k]
        S_inv_1 = Matrix(Fqm ,diff_n_k,diff_n_k,S_inv_1)
        rankS = rank(S_inv_1)
    return S_inv_1

def Random_Matrix(Fieldbase_q, length_m, num_row, num_col, seed_num = 0):
    Fqm = FiniteField(Fieldbase_q**length_m, 'z')
    Fq = FiniteField(Fieldbase_q)
    temp = zero_matrix(Fqm, num_row,num_col)
    set_random_seed(seed_num)
    for i in range(num_row):
        for j in range(num_col):
            temp[i,j] = Fqm.random_element()
    
    ran_mat = Matrix(Fqm, num_row,num_col,temp)
    return ran_mat 

def isometry_matrix(Fieldbase_q, length_m, length_n, seed_num = 0):
    rankP = 0
    set_random_seed(seed_num)
    while rankP != length_n :
        P_1 = Random_Matrix(Fieldbase_q,length_m,length_n,length_n)
        rankP = rank(P_1)
    return P_1

def Random_Generator_Matrix(Fieldbase_q, length_m, length_n, length_l, seed_num = 0):
    Fqm = FiniteField(Fieldbase_q**length_m, 'z')
    Fq = FiniteField(Fieldbase_q)
    rand_g = zero_matrix(Fqm, 1, length_n)
    set_random_seed(seed_num)
    for i in range(length_n):
        rand_g[0,i] = Fqm.random_element()
    g_vec = vector(rand_g)
    g_circ =  matrix.circulant(g_vec)
    Mat_G = g_circ[0:length_l,:]
    return Mat_G


def make_random_message(Fieldbase_q, length_m, vec_length, seed_num = 0):
    Fqm = FiniteField(Fieldbase_q**length_m, 'z')
    rand_g = zero_matrix(Fqm, 1, vec_length)
    set_random_seed(seed_num)
    for i in range(vec_length):
        rand_g[0,i] = Fqm.random_element()
    rand_vec = matrix(Fqm, rand_g)
    return rand_vec

def Hash_function(error_vec, length_n, length_k, length_l , seed_num = 0) : 
    import random
    random.seed(seed_num)
    index_list = range(2*length_n - length_k)
    index = random.sample(index_list, length_l)
    Field = error_vec.base_ring()
    Hash_error = zero_matrix(Field, 1, length_l)
    for i in range(length_l):
        Hash_error[0,i] = error_vec[0, index[i]] 
    return matrix(Hash_error)

def make_random_list(Fieldbase_q, length_m, length_t, seed_num = 0):
    Fqm = FiniteField(Fieldbase_q**length_m, 'z')
    rand_g_1 = zero_matrix(Fqm, 1, length_t)
    set_random_seed(seed_num)
    for i in range(length_t):
        rand_g_1[0,i] = Fqm.random_element()
    rand_list = matrix(Fqm, rand_g_1)
    return rand_list

def make_random_error(Fieldbase_q, length_m, length_n, length_k, length_t = t, seed_num = 0):
    Fqm = FiniteField(Fieldbase_q**length_m, 'z')
    rank_err  = 0
    while rank_err != length_t:
        enc_error_num = make_random_list(Fieldbase_q, length_m, length_t)
        RankMatrix_1 = zero_matrix(Fqm, length_m, length_t)
        for i in range(length_t):
            dis_mat = matrix(ZZ(enc_error_num[0,i].integer_representation() ).digits(base=Fieldbase_q, padto=length_m))
            for j in range(length_m):
                RankMatrix_1[j,i] = dis_mat[0,j]
        rank_err = rank(RankMatrix_1)
        
    import random
    random.seed(seed_num)
    index_list = range(2*length_n - length_k)
    index_out = random.sample(index_list, length_t)
    
    rand_err_vec = zero_matrix(Fqm, 1, 2*length_n - length_k)
    
    for i in range(length_t):
        rand_err_vec[0,index_out[i]] = enc_error_num[0,i]
        
    return rand_err_vec 

def recover_m_prime(matrix_G, cwd_1, err_1):
    Field = matrix_G.base_ring()
    NumofRows = matrix_G.nrows()
    matrix_G_tran = matrix_G.transpose()
    matrix_G_cwd_err = matrix_G_tran.augment(matrix(c_1 - e_1).transpose())
    matrix_G_cwd_err_rref = matrix(matrix_G_cwd_err.rref())
    matrix_G_rref_cut = matrix(matrix_G_cwd_err_rref[:,-1])
    m_prime = matrix(matrix_G_rref_cut[0:NumofRows, :])
    return m_prime.transpose()

def poly_to_bin(polynomial):
    p_coeff = matrix(ZZ(polynomial.integer_representation() ).digits(base=q, padto=m))
    p_bin = p_coeff[0]
    return p_bin

def merge_two_poly_to_bin(poly_bin_1, poly_bin_2):
    merge_poly_bin = matrix(poly_bin_1)
    add_poly_bin = matrix(poly_bin_2)
    merge_poly_bin = merge_poly_bin.augment(add_poly_bin)[0]
    return merge_poly_bin

def bin_to_hex(poly_mat):
    NumOfRow = poly_mat.nrows()
    NumOfCol = poly_mat.ncols() 
    Poly_Bin_vec = poly_to_bin(poly_mat[0,0])
    len_Poly_Bin_vec = len(Poly_Bin_vec)
    for i in range(NumOfRow):
        for j in range(NumOfCol):
            if i == 0 and j == 0:
                continue
            add_Poly_Bin_vec = zero_vector(GF(2), len_Poly_Bin_vec)
            add_Poly_Bin_vec = poly_to_bin(poly_mat[i,j])
            Poly_Bin_vec = merge_two_poly_to_bin(Poly_Bin_vec, add_Poly_Bin_vec)
    
    len_Poly_Bin_vec = len(Poly_Bin_vec)
    zero_vec = zero_vector(GF(2), 4)
    if len_Poly_Bin_vec % 4 == 0:
        Poly_Bin_vec = Poly_Bin_vec
    else:
        Poly_Bin_vec = merge_two_poly_to_bin(Poly_Bin_vec, zero_vec[0 : (4-len_Poly_Bin_vec % 4)])
    
    len_Poly_Bin_vec = len(Poly_Bin_vec)
    Hex_num = str()
    for i in range(len_Poly_Bin_vec//4):
        Hex_vec = str()
        if str(Poly_Bin_vec[4*i]) == str(0) and str(Poly_Bin_vec[4*i+1]) == str(0) and str(Poly_Bin_vec[4*i+2]) == str(0) and str(Poly_Bin_vec[4*i+3]) == str(0):
            Hex_num = Hex_num + '0'
        else:
            Hex_vec = str(Poly_Bin_vec[4*i])
            Hex_vec = Hex_vec +str(Poly_Bin_vec[4*i+1])
            Hex_vec = Hex_vec +str(Poly_Bin_vec[4*i+2])
            Hex_vec = Hex_vec +str(Poly_Bin_vec[4*i+3])
            add_Hex_num = Integer(int(str(Hex_vec), base=2)).digits(16,'0123456789ABCDEF')[0]
            Hex_num = Hex_num + add_Hex_num
    return Hex_num


# Key Gen

Fqm = FiniteField(q**m, 'z')
Fq = FiniteField(q)
p = Fq.characteristic()
Frob = Fqm.frobenius_endomorphism(log(q,p))
L = Fqm['z',Frob]

rankH2 = 0

while rankH2 != n-k :
    C_1 = GabidulinCode(Fq , Fqm, L, 2*n-k, n)
    G_gab_1 = matrix(C_1.generator_matrix())
    Parity_C_Gab_1 = C_1.dual_code()
    Parity_Gab_1 = matrix(Parity_C_Gab_1.generator_matrix())
    H_1 = Parity_Gab_1[:, :n]
    H_2 = Parity_Gab_1[:, n:]
    rankH2 = rank(H_2)

S_inv = nonsingular_matrix_inv(q, m, n, k, lambda_sp)

P = isometry_matrix(q,m,n)

G = Random_Generator_Matrix(q,m,n,l)

F = G * P.inverse() * H_1.transpose() * (H_2.transpose()).inverse() * S_inv.inverse()





pK = (G , F)
#pK_0 = bin_to_hex(pK[0])
#pK_1 = bin_to_hex(pK[1])
sK = (P, Parity_Gab_1, S_inv.inverse(), Parity_C_Gab_1.decoder("GabidulinGao"))
#sK_0 = bin_to_hex(sK[0])
#sK_1 = bin_to_hex(sK[1])
#sK_2 = bin_to_hex(sK[2])
random_seed = random.sample(range(10000,100000), 1)



# Encryption

seed = random_seed[0]
msg = make_random_message(q,m,l,seed)
t_error = make_random_error(q, m, n, k, t)
H_e = Hash_function(t_error,n,k,l)
m_prime = msg + H_e
enc_c_1 = m_prime * pK[0] + t_error[0, 0:n]
enc_c_2 = m_prime * pK[1]+ t_error[0, n:]
enc_c = enc_c_1.augment(enc_c_2)

# Decryption

dec_c = enc_c
dec_c_1 = dec_c[0, 0:n]
dec_c_2 = dec_c[0, n:]
dec_Parity_Gab = sK[1]
dec_H_1 = dec_Parity_Gab[:, :n]
dec_H_2 = dec_Parity_Gab[:, n:]
D_dec_c = sK[3].decode_to_code(dec_c[0])
e_1 = (matrix(dec_c[0] - D_dec_c))[0, 0:n]
c_1 = dec_c[0, 0:n]
dec_msg_prime = recover_m_prime(G, c_1, e_1)
dec_msg = dec_msg_prime - H_e


#print("count : ", i_count + (j * 10))
#print("seed : ", seed) 
#print("Message : " )
#print(msg)
#print(bin_to_hex(msg))
#print("Cipher text : " )
#print(bin_to_hex(enc_c))
#print("Decrypted text : " )
#print(bin_to_hex(dec_msg))


printFunc = c_module.printseed
printFunc.argtypes = [ctypes.c_int]
printFunc.restype = None

printFunc_1 = c_module.printmsg
printFunc_1.argtypes = [ctypes.c_wchar_p]
printFunc_1.restype = None
t_msg = bin_to_hex(msg)
c_msg = c_wchar_p(str(t_msg))
t_enc_msg = bin_to_hex(enc_c)
c_enc_msg = c_wchar_p(str(t_enc_msg))
t_dec_msg = bin_to_hex(dec_msg)
c_dec_msg = c_wchar_p(str(t_dec_msg))


printFunc(seed)
print("Message : " )
print(c_msg.value)
print("Cipher text : " )
print(c_enc_msg.value)
print("Decrypted text : " )
print(c_dec_msg.value)

        
