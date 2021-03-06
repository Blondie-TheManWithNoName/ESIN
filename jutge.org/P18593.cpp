#include <iostream>
#include <cstddef>
using namespace std;

template <typename T>
class Arbre {
private:
  Arbre(): _arrel(NULL) {};
  struct node {
    T info;
    node* primf;
    node* seggerm;
    node* ult;
  };
  node* _arrel;
  static node* copia_arbre(node* p);
  static void destrueix_arbre(node* p) throw(); 

public:
  // Construeix un Arbre format per un únic node que conté a x.
  Arbre(const T &x);

  // Tres grans.
  Arbre(const Arbre<T> &a);
  Arbre& operator=(const Arbre<T> &a);
  ~Arbre() throw();

  // Col·loca l'Arbre donat com a primer fill de l'arrel de l'arbre sobre el que s'aplica el mètode i l'arbre a queda invalidat; després de fer b.afegir_fill(a), a no és un arbre vàlid.
  void afegir_fill(Arbre<T> &a);

  // Iterador sobre arbre general.
  friend class iterador;
  class iterador {
  public:
    friend class Arbre;

    // Construeix un iterador no vàlid.
    iterador() throw();

    // Retorna el subarbre al que apunta l'iterador; llança un error si l'iterador no és vàlid.
    Arbre<T> arbre() const;

    // Retorna l'element del node al que apunta l'iterador o llança un error si l'iterador no és vàlid.
    T operator*() const;

    // Retornaiterador un iterador al primogenit del node al que apunta; llança un error si l'iterador no és vàlid.
	iterador primogenit() const;

    // Retorna un iterador al següent germà del node al que apunta; llança un error si l'iterador no és vàlid.
    iterador seg_germa() const;

    // Operadors de comparació.
    bool operator==(const iterador &it) const {
      return _p == it._p;
    };
    bool operator!=(const iterador &it) const {
      return _p != it._p;
    };
    static const int IteradorInvalid = 410;

  private:
    Arbre<T>::node* _p;
  };
    
  // Retorna un iterador al node arrel de l'Arbre (un iterador no vàlid si l'arbre no és vàlid).
  iterador arrel() const throw();

  // Retorna un iterador no vàlid.
  iterador final() const throw();

  static const int ArbreInvalid = 400;
};
// La còpia es fa seguint un recorregut en preordre.
template <typename T>				
typename Arbre<T>::node* Arbre<T>::copia_arbre(node* p) { 
  node* aux = NULL;
  if (p != NULL) {
    aux = new node;
    try {
      aux -> info = p -> info;
      aux -> primf = aux -> seggerm = NULL;
      aux -> primf = aux -> ult = NULL;
      aux -> primf = copia_arbre(p -> primf);
      aux -> seggerm = copia_arbre(p -> seggerm);
    }
    catch (...) {
      destrueix_arbre(aux);
    }
  }
  return aux;
}

// La destrucció es fa seguint un recorregut en postordre.
template <typename T>	
void Arbre<T>::destrueix_arbre(node* p) throw() { 
  if (p != NULL) {
    destrueix_arbre(p -> primf);
    destrueix_arbre(p -> seggerm);
    delete p;
  }
}

// Construcció d'un arbre que conté un sol element x a l'arrel.
template <typename T>	
Arbre<T>::Arbre(const T &x) {
  _arrel = new node; 
  try {
    _arrel -> info = x;
    _arrel -> seggerm = NULL;
    _arrel -> primf = NULL;
  } 
  catch (...) {
    delete _arrel;
    throw;
  }
}

template <typename T>	
Arbre<T>::Arbre(const Arbre<T> &a) { 
  _arrel = copia_arbre(a._arrel);
}

template <typename T>	
Arbre<T>&  Arbre<T>::operator=(const Arbre<T> &a) { 
  Arbre<T> tmp(a);
  node* aux = _arrel;
  _arrel = tmp._arrel;
  tmp._arrel = aux;
  return *this;
}

template <typename T>	
Arbre<T>::~Arbre() throw() { 
  destrueix_arbre(_arrel);
}

template <typename T> 
void Arbre<T>::afegir_fill(Arbre<T> &a) { 
  if (_arrel == NULL or a._arrel == NULL) {
    throw ArbreInvalid;
  }
  if (_arrel -> primf == NULL)
  {
    _arrel -> primf = a._arrel;
    _arrel -> ult = _arrel -> primf;
  }
  else
  {
    node* aux = _arrel;
    // _arrel->primf;
    //while (aux->seggerm != NULL)
      //aux = aux->seggerm;
    aux -> ult-> seggerm = a._arrel;
    aux -> ult = aux -> ult -> seggerm;
  }
  a._arrel = NULL;
}

template <typename T>
typename Arbre<T>::iterador Arbre<T>::arrel() const throw() { 
  iterador it;
  it._p = _arrel;
  return it;
}

template <typename T> 
typename Arbre<T>::iterador Arbre<T>::final() const throw() { 
  return iterador();
}

template <typename T>       
Arbre<T>::iterador::iterador() throw(): _p(NULL) { 
}

template <typename T> 
T Arbre<T>::iterador::operator*() const { 
  if (_p == NULL) {
    throw IteradorInvalid;
  }
  return _p -> info;
}

template <typename T> 
typename Arbre<T>::iterador Arbre<T>::iterador::primogenit() const { 
  if (_p == NULL) {
    throw IteradorInvalid;
  }
  iterador it;
  it._p = _p -> primf;
  return it;
}

template <typename T> 
typename Arbre<T>::iterador Arbre<T>::iterador::seg_germa() const { 
  if (_p == NULL) {
    throw IteradorInvalid;
  }
  iterador it;
  it._p = _p -> seggerm;
  return it;
}

template <typename T> 
Arbre<T> Arbre<T>::iterador::arbre() const { 
  if (_p == NULL) {
    throw IteradorInvalid;
  }
  Arbre<T> a;
  a._arrel = _p;
  Arbre<T> aux(a);
  a._arrel = NULL;
  return aux;
}
// Llegeix un arbre i el desa a un tros del vector v començant a la posicio j. 
// Modifica la variable j perque apunti a la seguent posicio lliure de v. 
// Retorna la posicio dins de v de l’arrel del (sub)arbre llegit. 


int crear_arbre (Arbre <int> &a, int j)	
{
	int x = j;
	++j;
	int f;
	cin >> f;
	for (int i = 0; i < f and f > 0; ++i)
		{				
			int g;
			cin >> g;
			Arbre <int> h(g);
			crear_arbre(h, i);
			a.afegir_fill(h);
		}
	return x;
}

void suma_aux(Arbre<int>::iterador it1, Arbre<int>::iterador end1, Arbre<int>::iterador it2, Arbre<int>::iterador end2)
{

	if (it1 != end1 and it2 != end1)
	{
		cout << " " << (*it1 + *it2);
		suma_aux(it1.primogenit(), end1, it2.primogenit(), end2);
		suma_aux(it1.seg_germa(), end1, it2.seg_germa(), end2);
	}
	else
	{
		if (it1 == end1 and it2 != end2)
		{
			cout << " " << *it2;
			suma_aux(it1, end1, it2.primogenit(), end2);
			suma_aux(it1, end1, it2.seg_germa(), end2);
		}
		else if (it2 == end2 and it1 != end1)
		{
			cout << " " << *it1;
			suma_aux(it1.primogenit(), end1, it2, end2);
			suma_aux(it1.seg_germa(), end1, it2, end2);

		}
	}
}
void suma(Arbre<int> &a, Arbre<int> b)
{
	suma_aux(a.arrel(), a.final(), b.arrel(), b.final());
}

void mostra (Arbre<int>::iterador it1, Arbre<int>::iterador end1)
{
	if (it1 != end1)
	{
		cout << " " << *it1;
		mostra(it1.primogenit(), end1);
		mostra(it1.seg_germa(), end1);
	}
}

int main()
{
	int n, m;
	int i = 0;
	cin >> n;
	if (n > 0)
	{
		cin >> m;
		Arbre <int> b(m);
		if (n >= 1)
			crear_arbre(b, i);

		cin >> n;
		if (n > 0)
		{
			cin >> m;
			Arbre <int> c(m);
			if (n > 1)
				crear_arbre(c, i);
			suma(b, c);
		}
		else
		{
			mostra(b.arrel(), b.final());
		}
	}
	else
	{
		cin >> n;
		if (n > 0)
		{
			cin >> m;
			Arbre <int> b(m);
			if (n >= 1)
				crear_arbre(b, i);
			mostra(b.arrel(), b.final());
		}
	}
	cout << endl;
} 
 
