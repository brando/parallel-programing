import java.util.ArrayList;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Program 
{
	public static final int T_CHORI_COUNT = 3;
	public static final int L_CHORI_COUNT = 7;
	
	public static final long MAIN_SLEEP = 20000;
	public static final long SLEEP_TAZKO_CHORY = 1000;
	public static final long SLEEP_LAHKO_CHORY = 500;
	public static final long LAHKO_CHORY_DOMA = 3000;
	public static final long TAZKO_CHORY_DOMA = 2500;
	
	public static  boolean STOP = true;
	
	public static int count_tazko_chori = 0;
	public static int count_lahko_chori = 0;
	public static int count_vsetci = 0;
	public static int pritomni_t = 0;
	public static int pritomni_l = 0;
	
	public static ReentrantLock mutex_tazko_chori = new ReentrantLock();
	public static ReentrantLock mutex_lahko_chori = new ReentrantLock();
	public static ReentrantLock mutex_vysetrenie = new ReentrantLock();

	
	public static Condition cond_t = mutex_vysetrenie.newCondition();
	public static Condition cond_l = mutex_vysetrenie.newCondition();

	public static void main(String[] args) 
	{
		ArrayList<Thread> chori = new ArrayList<Thread>();
		
		int i, j; 
		
		for(i = 0; i < Program.T_CHORI_COUNT; i++)
		{
			Thread t_chory = new TazkoChory(i);
			t_chory.start();
			chori.add(t_chory);
		}
		
		for(j = 0; j < Program.L_CHORI_COUNT; j++)
		{
			LahkoChory l_chory = new LahkoChory(j);
			l_chory.start();
			chori.add(l_chory);
		}
		
		try {
			Thread.sleep(Program.MAIN_SLEEP);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		Program.safePrintln("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! main thread awaken !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//		System.out.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! main thread awaken !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		Program.mutex_vysetrenie.lock();  //potrebujem lock na cond_l & cond_t 
		Program.cond_l.signalAll();
		Program.cond_t.signalAll();
		Program.STOP = false;  	
		Program.mutex_vysetrenie.unlock();
		
		for(Thread ch : chori){
	    	try {
				ch.join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	    }
		
		Program.safePrintln("pocet vysetreni tazko chorych: " + Program.count_tazko_chori + ", lahko chorych " + Program.count_lahko_chori + ", vsetkcyh " + Program.count_vsetci);
//		System.out.println("pocet vysetreni tazko chorych: " + Program.count_tazko_chori + ", lahko chorych " + Program.count_lahko_chori + ", vsetkcyh " + Program.count_vsetci);
	}
	
	//synchronizovany vypismo
	public static synchronized void safePrintln(String s) 
	{
  	    System.out.println(s);
	}

}
