/*  Rubik's Pocket Cube Solver
    Author: Varan Shukla

    Call the six sides, as usual:
       Front Back   Up Down   Left Right
    or F, B, U, D, L, R.
    Permutations:
    We'll number the cubie positions starting
    at 0, front to back, up to down, left to right.
    We give an alphabetic name to the cubies as well,
    by listing the faces it contains, starting with
    F or B, in clockwise order (looking in from outside).
       0th cubie = FLU
       1st cubie = FUR
       2nd cubie = FDL
       3rd cubie = FRD
       4th cubie = BUL
       5th cubie = BRU
       6th cubie = BLD
       7th cubie = BDR
    Each cubie has three faces, so we have 24 face
    positions.  We'll label them as 0 to 23, but also
    with a 3-letter name that specifies the name
    of the cubie it is on, cyclically rotated to
    put the name of the face first (so cubie FLU
    has faces flu, luf, and ufl, on sides F, L,
    and U, respectively). We'll use lower case
    here for clarity.  Here are the face names,
    written as variables for later convenience.
    We also save each number in a second variable,
    where the positions are replaced by the colors that
    would be there if the cube were solved and had its
    orange-yellow-blue cubie in position 7, with yellow
    facing down.
    flu refers to the front face (because f is first) of the cubie that
    has a front face, a left face, and an upper face.
    yob refers to the colors yellow, orange, blue that are on the
    respective faces if the cube is in the solved position.*/

#include<iostream>
#include<array>
#include<set>
#include<unordered_map>
#include<queue>
#include<utility>
using namespace std;

int rgw = 0, flu = 0;
int gwr = 1, luf = 1;
int wrg = 2, ufl = 2;

int rwb = 3, fur = 3;
int wbr = 4, urf = 4;
int brw = 5, rfu = 5;

int ryg = 6, fdl = 6;
int ygr = 7, dlf = 7;
int gry = 8, lfd = 8;

int rby = 9, frd = 9;
int byr = 10, rdf = 10;
int yrb = 11, dfr = 11;

int owg = 12, bul = 12;
int wgo = 13, ulb = 13;
int gow = 14, lbu = 14;

int obw = 15, bru = 15;
int bwo = 16, rub = 16;
int wob = 17, ubr = 17;

int ogy = 18, bld = 18;
int gyo = 19, ldb = 19;
int yog = 20, dbl = 20;

int oyb = 21, bdr = 21;
int ybo = 22, drb = 22;
int boy = 23, rbd = 23;

/*A permutation p on 0,1,...,n-1 is represented as
a list of length n-1.  p[i] = j means of course
that p maps i to j.
When operating on a list c (e.g. a list of length
24 of colors), then  p * c
is the rearranged list of colors:
   (p * c)[i] = c[p[i]]    for all i
Thus, p[i] is the location of where the color of
position i will come from; p[i] = j means that
the color at position j moves to position i.*/

array<int, 24> perm_inverse(array<int, 24> p){
	array<int, 24> inv;
	for(int i=0; i<24; i++){
		inv[p[i]] = i;
	}
	return inv;
}

array<int, 24> I = {flu, luf, ufl, fur, urf, rfu, fdl, dlf, lfd, frd, rdf, dfr,
					 bul, ulb, lbu, bru, rub, ubr, bld, ldb, dbl, bdr, drb, rbd};

array<int, 24> F = {fdl, dlf, lfd, flu, luf, ufl, frd, rdf, dfr, fur, urf, rfu,
					 bul, ulb, lbu, bru, rub, ubr, bld, ldb, dbl, bdr, drb, rbd};

array<int, 24> Fi = perm_inverse(F);

array<int, 24> L = {ulb, lbu, bul, fur, urf, rfu, ufl, flu, luf, frd, rdf, dfr,
					 dbl, bld, ldb, bru, rub, ubr, dlf, lfd, fdl, bdr, drb, rbd};

array<int, 24> Li = perm_inverse(L);

array<int, 24> U = {rfu, fur, urf, rub, ubr, bru, fdl, dlf, lfd, frd, rdf, dfr,
					 luf, ufl, flu, lbu, bul, ulb, bld, ldb, dbl, bdr, drb, rbd};

array<int, 24> Ui = perm_inverse(U);

set<array<int, 24>> quarter_twists = {F, Fi, L, Li, U, Ui};

array<int, 24> perm_apply(array<int, 24> position, array<int, 24> perm){
	array<int, 24> new_position;
	for(int i=0;i<24;i++){
		new_position[i] = position[perm[i]]; 
	}
	return new_position;
}

inline char permToChar(array<int, 24> p){
	if(p == F)
		return 'F';
	else if(p == Fi)
		return 'f';
	else if(p == L)
		return 'L';
	else if(p == Li)
		return 'l';
	else if(p == U)
		return 'U';
	else if(p == Ui)
		return 'u';
	else return 'n';
}

inline array<int, 24> charToPerm(char p){
	if(p == 'F')
		return F;
	else if(p == 'f')
		return Fi;
	else if(p == 'L')
		return L;
	else if(p == 'l')
		return Li;
	else if(p == 'U')
		return U;
	else if(p == 'u')
		return Ui;
}

inline string charToMove(char c){
	if(c == 'F')
		return "F ";
	else if(c == 'f')
		return "Fi ";
	else if(c == 'L')
		return "L ";
	else if(c == 'l')
		return "Li ";
	else if(c == 'U')
		return "U ";
	else if(c == 'u')
		return "Ui ";
}

inline string invCharToMove(char c){
	if(c == 'F')
		return "Fi ";
	else if(c == 'f')
		return "F ";
	else if(c == 'L')
		return "Li ";
	else if(c == 'l')
		return "L ";
	else if(c == 'U')
		return "Ui ";
	else if(c == 'u')
		return "U ";
}

//Hashing functions used to hash the fixed size array containers
void hash_combine(std::size_t& seed, std::size_t value){
	seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct container_hasher{
	template<class T>
	std::size_t operator()(const T& c) const{
		std::size_t seed=0;
		for(const auto& elem : c){
			hash_combine(seed, std:: hash<typename T::value_type>()(elem));
		}
		return seed;
	}
};

string shortest_path(array<int, 24> start, array<int, 24> end){
	if(start == end)
		return "Already Solved!";
	unordered_map<array<int, 24>, char, container_hasher> forward_parents;
	forward_parents[start] = 'N';
	unordered_map<array<int, 24>, char, container_hasher> backward_parents;
	backward_parents[end] = 'N';
	queue< pair<array<int, 24>, bool>> Q;
	Q.push(make_pair(start, true));
	Q.push(make_pair(end, false));

	while(true){
		if(Q.empty())
			return "No Solution!";
		pair<array<int, 24>, bool> vertex = Q.front();
		array<int, 24> position = vertex.first;
		Q.pop();
		if(vertex.second==true){
			set<array<int, 24>>::iterator move;
			for(move = quarter_twists.begin(); move != quarter_twists.end(); ++move){
				array<int, 24> next_postition = perm_apply(position, *move);
				if(forward_parents.find(next_postition) != forward_parents.end()){
					continue;
				}
				forward_parents[next_postition] = permToChar(*move);
				Q.push(make_pair(next_postition, true));
				if(backward_parents.find(next_postition) != backward_parents.end()){
					string solution;
					array<int, 24> temp = next_postition;
					char x = forward_parents[temp];
					while(x != 'N'){
						solution = charToMove(x) + solution;
						temp = perm_apply(temp, perm_inverse(charToPerm(x)));
						x = forward_parents[temp];
					}
					temp = next_postition;
					x = backward_parents[temp];
					while(x != 'N'){
						solution = solution + invCharToMove(x);
						temp = perm_apply(temp, perm_inverse(charToPerm(x)));
						x = backward_parents[temp];
					}
					return solution;
				}
			}	
		}
		else{
			set<array<int, 24>>::iterator move;
			for(move = quarter_twists.begin(); move != quarter_twists.end(); ++move){
				array<int, 24> next_postition = perm_apply(position, *move);
				if(backward_parents.find(next_postition) != backward_parents.end()){
					continue;
				}
				backward_parents[next_postition] = permToChar(*move);
				Q.push(make_pair(next_postition, false));
				if(forward_parents.find(next_postition) != forward_parents.end()){
					string solution;
					array<int, 24> temp = next_postition;
					char x = forward_parents[temp];
					while(x != 'N'){
						solution = charToMove(x) + solution;
						temp = perm_apply(temp, perm_inverse(charToPerm(x)));
						x = forward_parents[temp];
					}
					temp = next_postition;
					x = backward_parents[temp];
					while(x != 'N'){
						solution = solution + invCharToMove(x);
						temp = perm_apply(temp, perm_inverse(charToPerm(x)));
						x = backward_parents[temp];
					}
					return solution;
				}
			}
		}
	}
 }

int main(){
	//Couple of test cases:
	//14 Length: {6, 7, 8, 20, 18, 19, 3, 4, 5, 16, 17, 15, 0, 1, 2, 14, 12, 13, 10, 11, 9, 21, 22, 23};
	//No Solution: {7, 8, 6, 20, 18, 19, 3, 4, 5, 16, 17, 15, 0, 1, 2, 14, 12, 13, 10, 11, 9, 21, 22, 23};
	array<int, 24> start = {6, 7, 8, 20, 18, 19, 3, 4, 5, 16, 17, 15, 0, 1, 2, 14, 12, 13, 10, 11, 9, 21, 22, 23};
	string solution = shortest_path(start, I);
	cout<<solution<<endl;
	return 0;
}