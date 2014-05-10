#include <stdio.h>
#include <stdlib.h>
#define BLACK 1
#define RED 0

typedef int tipo_chave;
typedef char tipo_cor;

typedef struct nodo{
  tipo_chave chave;
  tipo_cor cor;
  struct nodo *esq, *dir,*pai;
} nodo;

typedef struct RBTree{
	struct nodo *raiz;
	struct nodo *nil;
}RBTree;

nodo *Consulta(RBTree *t, nodo *p , int k);
void ImprimaOrdenado(RBTree *t,nodo *p);
nodo *Minimo(RBTree *t, nodo *p);
nodo *Maximo(RBTree *t,nodo *p);
void RotEsq(RBTree *t, nodo *p);
void RotDir(RBTree *t, nodo *p);
void Insira(RBTree *t,int k);
void conserte_insercao(RBTree *t,nodo *p);
void Transplante(RBTree *t,nodo *p, nodo *q);
void Delete( RBTree *t, nodo *p);
 void conserte_delecao(RBTree *t, nodo *x);
 nodo* SUCESSOR(RBTree *t,nodo *x);
 nodo* ANTECESSOR(RBTree *t,nodo *x);
 int altura(RBTree *t,nodo *p);
int nodos(RBTree *t, nodo *p);
int    balanceada(RBTree *t, nodo *p);

int nivelChave(RBTree *t, nodo *p, int k, int nivel){
    int result;

    if(p == t->nil){
        return -1;
    }else if(p->chave == k ){
        return nivel;
    }else{
        result = nivelChave(t,p->esq,k,nivel+1);
        if( result >=0 )
            return result;
        else
            return nivelChave(t,p->dir,k, nivel+1);

    }
}

int altura(RBTree *t,nodo *p){
    if(p == t->nil)
        return 0;

    int altE =  altura(t,p->esq);
    int altD = altura(t,p->dir);

    if(altE > altD)
        return altE + 1;

    return (altD + 1);
}

int    balanceada(RBTree *t, nodo *p){
    if (p == t->nil){
        return 1;
    }else if (p->esq == t->nil && p->dir== t->nil){ /* r não tem filhos */
        return 1;
    }else if(p->esq != t->nil && p->dir != t->nil){ /* r tem ambas subárvores não-nulas */
        if ( balanceada(t, p->esq) && balanceada(t, p->dir) ){
            return 1;
        }else{
            return 0;
        }
    }else if(p->esq != t->nil){/* tem um único filho –`a esquerda */
        if( altura(t, p->esq) == 1 ){
            return 1;
        }else{
            return 0;
        }
    }else{ /* tem um único filho –`a direita */
        if( altura(t, p->esq) == 1){
            return 1 ;
        }else{
            return 0;
        }
    }
}

nodo *Consulta(RBTree *t, nodo *p , int k){

	while( p != t->nil && k != p->chave){
        if(k < p->chave )
            p = p->esq;
        else
            p = p->dir;
	}
	return p;

}

void ImprimaOrdenado(RBTree *t,nodo *p){
	if( p != t->nil){
		ImprimaOrdenado(t,p->esq);
		printf("%d ",p->chave);
		if(p->cor == BLACK)
            printf("PRETO\n");
        else
            printf("VERMELHO\n");
		ImprimaOrdenado(t,p->dir);
	}
}

nodo *Minimo(RBTree *t, nodo *p){
	while (p->esq != t->nil){
		p = p->esq;
	}
	return p;
}

nodo *Maximo(RBTree *t,nodo *p){
	while( p->dir != t->nil){
        p = p->dir;
	}
    return p;
}

void RotEsq(RBTree *t, nodo *p){
    nodo *q;
    q = p->dir;
    p->dir = q->esq;
    if(p->dir != t->nil)
        p->dir->pai = p;
    q->pai = p->pai;
    if(q->pai == t->nil)
        t->raiz = q;
    else if(p == p->pai->esq)
        p->pai->esq = q;
    else p->pai->dir = q;

    q->esq = p;
    p->pai = q;
}

void RotDir(RBTree *t, nodo *p){
    nodo *q;
    q = p->esq;
    p->esq = q->dir;
    if(p->esq != t->nil)
        p->esq->pai = p;
    q->pai = p->pai;
    if(q->pai == t->nil)
        t->raiz = q;
    else if(p == p->pai->dir)
        p->pai->dir = q;
    else p->pai->esq = q;

    q->dir = p;
    p->pai = q;
}
void Insira(RBTree *t,int k){

	nodo *x = (nodo*)malloc(sizeof(nodo));
	nodo *y, *yAnt;
	x->chave = k;
	x->pai = x->esq = x->dir = t->nil;
	x->cor = RED;

	y = t->raiz;
	yAnt = t->nil;
	while( y != t->nil){
		yAnt = y;
		if( k < y->chave)
			y = y->esq;
		else
			y = y->dir;
	}

	if (yAnt == t->nil)
		t->raiz = x;
	else if( k < yAnt->chave)
		yAnt->esq = x;
	else
		yAnt->dir =x;
	x->pai = yAnt;

 	  conserte_insercao(t,x);
}

void conserte_insercao(RBTree *t,nodo *p){
    nodo *y;
    while( p->pai->cor == RED){
        if(p->pai == p->pai->pai->esq){
            y = p->pai->pai->dir;
            if(y->cor == RED){
                p->pai->cor = BLACK;
                y->cor = BLACK;
                p->pai->pai->cor = RED;
                p = p->pai->pai;
            }else{
		if( p == p->pai->dir)
                  p = p->pai;  RotEsq(t,p);

                p->pai->cor = BLACK;
                p->pai->pai->cor = RED;
                RotDir(t,p->pai->pai);
            }
        }else{
            y = p->pai->pai->esq;
            if(y->cor == RED){
                p->pai->cor = BLACK;
                y->cor = BLACK;
                p->pai->pai->cor = RED;
                p = p->pai->pai;
            }else{
                if(p == p->pai->esq)
                    p = p->pai;  RotDir(t,p);
                p->pai->cor = BLACK;
                p->pai->pai->cor =RED;
                RotEsq(t,p->pai->pai);
            }
        }
    }
    t->raiz->cor = BLACK;
}

void Transplante(RBTree *t,nodo *p, nodo *q){
	if( p->pai == t->nil)
		t->raiz = q;
	else if( p == p->pai->esq)
		p->pai->esq = q;
	else
		p->pai->dir = q;
	if(q != t->nil)
		q->pai = p->pai;

}

void Delete( RBTree *t, nodo *p){
	nodo *y,*x;
	y = p;
	int cor_original_de_y = y->cor;

	if( p->esq == t->nil){
        x = p->dir;
		Transplante(t,p,p->dir);
	}else if( p->dir == t->nil){
	    x = p->esq;
		Transplante(t, p, p->esq);
	}else{
		y = Minimo(t,p->dir);
        cor_original_de_y = y->cor;
        x = y->dir;

		if( y->pai != p){
			Transplante(t,y,y->dir);
			y->dir = p->dir;
			p->dir->pai = y;
		}else{
            x->pai = y;
		}
		Transplante(t,p,y);
		y->esq = p->esq;
		p->esq->pai = y;
		y->cor = p->cor;
	}
	free(p);
    if(cor_original_de_y == BLACK) {
        conserte_delecao(t,x);
    }
}

 void conserte_delecao(RBTree *t, nodo *x){
    nodo *w;
    while(x != t->raiz && x->cor == BLACK){
        if(x == x->pai->esq){
            w = x->pai->dir;
            if (w->cor == RED){
                w->cor = BLACK;
                x->pai->cor = RED;
                RotEsq(t,x->pai);
                w = x->pai->dir;
            }
            if(w->esq->cor == BLACK && w->dir->cor == BLACK){
                w->cor = RED;
                x = x->pai;
            }else{
                if(w->dir->cor == BLACK){
                    w->esq->cor = BLACK;
                    w->cor = RED;
                    RotDir(t,w);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->dir->cor = BLACK;
                RotEsq(t,x->pai);
                x = t->raiz;
            }

        }else{
            w = x->pai->esq;
            if (w->cor == RED){
                w->cor = BLACK;
                x->pai->cor = RED;
                RotDir(t,x->pai);
                w = x->pai->esq;
            }
            if(w->dir->cor == BLACK && w->esq->cor == BLACK){
                w->cor = RED;
                x = x->pai;
            }else{
                if(w->esq->cor == BLACK){
                    w->dir->cor = BLACK;
                    w->cor = RED;
                    RotEsq(t,w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->esq->cor = BLACK;
                RotDir(t,x->pai);
                x = t->raiz;
            }
        }
    x->cor = BLACK;
    }
 }

nodo* SUCESSOR(RBTree *t,nodo *x){
    nodo *y;
    if (x->dir != t->nil)
        return Minimo(t,x->dir);
    y = x->pai;
    while (y != t->nil && x == y->dir){
		x = y;
         y = y->pai;
    }
	return y;
}

nodo* ANTECESSOR(RBTree *t, nodo *x){
    nodo *y;

    if (x->esq != t->nil)
        return Maximo(t,x->esq);
    y = x;
    while (y != t->nil && x == y->esq)
		x = y;
	y = y->pai;
	return y;
}

int nodos(RBTree *t, nodo *p){
    if (p == t->nil)
        return 0;
    int nE = nodos(t , p->esq);
    int nD = nodos(t ,p->dir);
    return(nE + nD + 1);
}


int main(){
	RBTree *t = (RBTree *)malloc(sizeof(RBTree));
	t->nil = (nodo*)malloc(sizeof(nodo));

        t->nil->pai = t->nil->dir = t->nil->esq = NULL;
	t->nil->cor = BLACK;

	t->raiz = t->nil;

	nodo *temp;
	char op;
	int dado, h, b , n , nivel;
	while(1){
		scanf("%c",&op);
		switch (op){
            case 'i':
                scanf(" %d",&dado);
                Insira(t, dado);
            break;

            case 'd':
                scanf("%d",&dado);
                temp = Consulta(t,t->raiz , dado);
                if(temp != t->nil){
                    Delete(t,temp);
                }
            break;

            case 'D':
                scanf("%d",&dado);
                do{
                 temp = Consulta(t,t->raiz , dado);
                if(temp != t->nil){
                    Delete(t,temp);
                }
                }while(temp != t->nil);

            break;

            case 's':
                scanf("%d",&dado);
                temp = Consulta(t,t->raiz , dado);
                if(temp != t->nil){
                    printf("%p\n",temp);
                }
            break;

            case 'p':
                ImprimaOrdenado(t,t->raiz);
                printf("\n");
            break;

            case '+':
                scanf("%d",&dado);
                temp = Consulta(t,t->raiz , dado);
                if(temp != t->nil && temp != t->raiz){
                    temp = SUCESSOR(t,temp);
                    if(temp != t->nil)
                        printf("%p\n",temp);
                }
            break;

            case '-':
                scanf("%d",&dado);
                temp = Consulta(t,t->raiz , dado);
                if(temp != t->nil && temp != t->raiz){
                    temp = ANTECESSOR(t,temp);
                    if(temp != t->nil)
                    printf("%p\n",temp);
                }
            break;

            case 'M':
                if(t->raiz != t->nil){
                    temp = Maximo(t,t->raiz);
                    printf("%d\n",temp->chave);
                }
            break;

            case 'm':
                if(t->raiz != t->nil){
                    temp = Minimo(t,t->raiz);
                    printf("%d\n",temp->chave);
                }
            break;

            case 'q':
                return 0;
            break;

            case 'h':
                h = altura(t, t->raiz);
                printf("h = %d\n",h);
            break;

            case 'b':
               b =  balanceada(t, t->raiz);
               if (b == 1){
                   printf("SIM\n");
               }else{
                    printf("NAO\n");
               }
            break;

            case 'n':
                n = nodos(t, t->raiz);
                printf("%d nodos\n",n);
            break;

            case 'N':
                scanf("%d",&dado);
                nivel = nivelChave(t,t->raiz,dado,0);
                printf("NIVEL DA CHAVE %d = %d\n",dado,nivel);
            break;

        }
    }
	return 0;
}
