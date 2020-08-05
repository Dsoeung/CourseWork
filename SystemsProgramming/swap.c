void decode(int *xp, int *yp, int *zp){
int t0 = *xp;
int t1 = *yp;
int t2 = *zp;

*xp = t2; 	//0
*yp = t1;	//1
*zp = t0;	//2
}