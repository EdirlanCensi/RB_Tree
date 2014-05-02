#include <stdio.h>
#include <stdlib.h>
#define BLACK 1
#define RED 0

typedef struct nodo{
  int chave;
  char cor;
  struct nodo *esq, *dir,*pai;
} nodo;

typedef struct BsTree{
	struct nodo *raiz;
	struct nodo *nil;
}BsTree;

nodo *Consulta(nodo *p , int k){

	while( p != NULL && k != p->chave){
        if(k < p->chave )
            p = p->esq;
        else
            p = p->dir;
	}
	return p;

}

void ImprimaOrdenado(nodo *p){
	if( p!= NULL){
		ImprimaOrdenado(p->esq);
		printf("%d ",p->chave);
		ImprimaOrdenado(p->dir);
	}
}

nodo *Minimo(nodo *p){
	while (p->esq != NULL){
		p = p->esq;
	}
	return p;
}

nodo *Maximo(nodo *p){
	while( p->dir != NULL){
        p = p->dir;
	}
    return p;
}

void RotEsq(BsTree *t, nodo *p){
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

void RotDir(BsTree *t, nodo *p){
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
void Insira(BsTree *t,int k){

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

void conserte_insercao(BsTree *t,nodo *p){
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
                if( p == p->pai->dir){
                    p = p->pai;
                    RotEsq(t,p);
                }
                p->pai->cor = BLACK;
                p->pai->pai->cor = RED;
                RotDir(t,p->pai->pai);
            }
        }else{
            y = p->pai->esq;
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

void Transplante(BsTree *t,nodo *p, nodo *q){
	if( p->pai == t->nil)
		t->raiz = q;
	else if( p == p->pai->esq)
		p->pai->esq = q;
	else
		p->pai->dir = q;
    q->pai = p->pai;

}

void Delete( BsTree *t, nodo *p){
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
		y = Minimo(p->dir);
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

 void conserte_delecao(BsTree *t, nodo *x){
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
                    w = x->pai->cor;
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
                    w = x->pai->cor;
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

nodo* SUCESSOR(nodo *x){
    nodo *y;
    if (x->dir != NULL)
        return Minimo(x->dir);
    y = x->pai;
    while (y != NULL && x == y->dir){
		x = y;
         y = y->pai;
    }
	return y;
}

nodo* ANTECESSOR(nodo *x){
    nodo *y;

    if (x->esq != NULL)
        return Maximo(x->esq);
    y = x;
    while (y != NULL && x == y->esq)
		x = y;
	y = y->pai;
	return y;
}



int main(){
	BsTree *t = (BsTree *)malloc(sizeof(BsTree));
	t->nil = (nodo*)malloc(sizeof(nodo));
	t->raiz = t->nil;
	t->nil->cor = BLACK;
	t->raiz->cor = BLACK;

	nodo *temp;
	char op;
	int dado;
	while(1){
		scanf("%c",&op);
		switch (op){
            case 'i':
                scanf("%d",&dado);
                Insira(t, dado);
            break;

            case 'd':
                scanf("%d",&dado);
                temp = Consulta(t->raiz , dado);
                if(temp != NULL){
                    Delete(t,temp);
                }
            break;

            case 'D':
                scanf("%d",&dado);
                do{
                 temp = Consulta(t->raiz , dado);
                if(temp != NULL){
                    Delete(t,temp);
                }
                }while(temp != NULL);

            break;

            case 's':
                scanf("%d",&dado);
                temp = Consulta(t->raiz , dado);
                if(temp != NULL){
                    printf("%p",temp);
                }
            break;

            case 'p':
                ImprimaOrdenado(t->raiz);
            break;

            case '+':
                scanf("%d",&dado);
                temp = Consulta(t->raiz , dado);
                if(temp != NULL ){
                    temp = SUCESSOR(temp);
                    if(temp != NULL)
                        printf("%p\n",temp);
                }
            break;

            case '-':
                scanf("%d",&dado);
                temp = Consulta(t->raiz , dado);
                if(temp != NULL && temp != t->raiz){
                    temp = ANTECESSOR(temp);
                    if(temp != NULL)
                    printf("%p\n",temp);
                }
            break;

            case 'M':
                if(t->raiz != NULL){
                    temp = Maximo(t->raiz);
                    printf("%d\n",temp->chave);
                }
            break;

            case 'm':
                if(t->raiz != NULL){
                    temp = Minimo(t->raiz);
                    printf("%d\n",temp->chave);
                }
            break;

            case 'q':
                return 0;
            break;
        }
    }
	return 0;
}
