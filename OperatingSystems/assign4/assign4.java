import java.util.*;

    class SelectionSort extends Thread {
        private double [] dArr;
        private int iStartingV;
        private int iEndingV;

        public SelectionSort(double [] arr, int startingV, int endingV) {
            dArr = arr;
            iStartingV = startingV;
            iEndingV = endingV;
        }

        public void run() {
            int i; 
            int j; 
            int min;
            double tmp;
            dArr = dArr;
            for(i = iStartingV; i < iEndingV; i++) {
                min = i;
                for(j = i + 1; j < iEndingV; j++) {
                    if(dArr[j] < dArr[min])
                        min = j;
                }
                swap(dArr[i],dArr[min]);
            }
        }
        
        public void swap(double dpArrA, double dpArrB) {
            double tmp;
            tmp = dpArrA;
            dpArrA = dpArrB;
            dpArrB = tmp;
        }
    };
    
	    class merge extends Thread {
        private double [] dInArr; 
        private double [] dOutArr;
        private int iMid;

        public merge(double [] iArr, double [] oArr, int m) {
            dInArr = iArr;
            dOutArr = oArr;
            iMid = m;
        }

        public void run() {
            int i = 0;
            int j;
            int k = 0;
            j = iMid;
            while(i < iMid && j < dInArr.length) {
                if(dInArr[i] < dInArr[j])
                    dOutArr[k++] = dInArr[i++];
                else
                    dOutArr[k++] = dInArr[j++];
            }
            while(i < iMid)
                dOutArr[k++] = dInArr[i++];
            while(j < dInArr.length)
                dOutArr[k++] = dInArr[j++];
        }
    };
	
public class assign4 {

    public static void main(String []arg) {
        double [] aArr; //A
        double [] bArr; //B
        double [] cArr; //C
        int mid;
        int iNput;
        int i;
        long start;
        long end;

        if(arg.length != 1) {
            System.out.println("ERROR USAGE: java assign4 (Integer)");
            return;
        }

        //iNput will be given as command line argument
        iNput = Integer.parseInt(arg[0]);
        
        //create array A ( n double values) and randomly generate these values 
        aArr = new double[iNput];
        
        //B same size of A
        bArr = new double[iNput];
        
        //C same size of A
        cArr = new double[iNput];
        
        
        Random rand = new Random(System.currentTimeMillis());

        for(i = 0; i < iNput; i++) { 
            aArr[i] = rand.nextDouble() * 100.0; 
        }

        //Create threads
        //Afirsthalf
        SelectionSort thA1 = new SelectionSort(aArr, 0, iNput/2);
        
        //AsecondHalf
        SelectionSort thA2 = new SelectionSort(aArr, iNput/2, aArr.length);
        
        //copy A into B
        for(i = 0; i < iNput; i++) { 
            aArr[i] = bArr[i];
        }
        
        //create thM  mergeThread to merge Afirsthalf and Asecondhalf into C
        merge thM = new merge(aArr, cArr, iNput/2);
        
        //doing two threads first for the sake of the example output
        //start = clock();
        start = System.nanoTime(); 

        thA1.start();
        thA2.start();

        try {
            //join thA1
            thA1.join();
            //join thA2
            thA2.join();
        } catch (Exception e) {
            e.printStackTrace();
        }

        //create thM  mergeThread to merge Afirsthalf and Asecondhalf into C
        thM.start();
        try {
            //join thM
            thM.join();
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        //Sorting is done in (end - start) ms when two threads are used 
        //End = clock() - start;
        end = System.nanoTime() - start; 

        System.out.println("Sorting is done in " + end/1000000.0f + "ms when two threads are used");
       
        //Time for single thread
         //create thB sortThread to sort B
        SelectionSort thB = new SelectionSort(bArr, 0, iNput);
        start = System.nanoTime(); 
        
        //start = clock();
        thB.start();
        try {
            thB.join();
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        //end = clock() - start;
        end = System.nanoTime() - start; 

        System.out.println("Sorting is done in "+ (end/1000000.0f) + "ms when one thread is used");
    }
}