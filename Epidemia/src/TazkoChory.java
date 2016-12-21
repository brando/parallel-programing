
public class TazkoChory extends Thread 
{
	private int i;
	public TazkoChory(int i) 
	{
		this.i= i;
	}

	@Override
	public void run() 
	{
		while(Program.STOP)
		{
			try {
				this.vysetrenie_tazko_choreho();
				this.volno_tazko_chory();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	private void vysetrenie_tazko_choreho() throws InterruptedException
	{
		Program.mutex_vysetrenie.lock(); //nemozem pouzit synchronized ked pouzivam reenatant lock !!!
		
		Program.pritomni_t++;
		Program.safePrintln(this.i + " tazko chory sa sa dostavil pred miestnost");
//		System.out.println(this.i + " tazko chory sa sa dostavil pred miestnost");
		
		while(Program.pritomni_l != 0)
			Program.cond_t.await();
		Program.safePrintln(this.i + " tazko chory bude vstupovat do miestnosti");
//		System.out.println(this.i + " tazko chory bude vstupovat do miestnosti");
		
		Program.mutex_vysetrenie.unlock();
			
		
		synchronized(Program.mutex_tazko_chori)
		{
			Program.safePrintln("vysetrenie " + this.i + " tazko choreho");
//			System.out.println("vysetrenie " + this.i + " tazko choreho");
			Thread.sleep(Program.SLEEP_TAZKO_CHORY);
			Program.count_tazko_chori++;
			Program.count_vsetci++;
		}
				
		Program.mutex_vysetrenie.lock(); 
		Program.pritomni_t--;
		Program.safePrintln("tazko chory " + this.i + " opustuje miestnost");
//		System.out.println("tazko chory " + this.i + " opustuje miestnost");
		Program.cond_l.signalAll();
		Program.mutex_vysetrenie.unlock();
	}
	
	private void volno_tazko_chory() throws InterruptedException
	{
		Program.safePrintln("tazko chory pacient " + this.i + " je doma");
//		System.out.println("tazko chory pacient " + this.i + " je doma");
		Thread.sleep(Program.TAZKO_CHORY_DOMA);
	}
}
