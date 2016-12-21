
public class LahkoChory extends Thread
{
	private int j;
	public LahkoChory(int j) 
	{
		this.j = j;
	}

	@Override
	public void run() 
	{
		while(Program.STOP)
		{
			try {
				this.vysetrenie_lahko_choreho();
				this.volno_lahko_chory();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}

	private void vysetrenie_lahko_choreho() throws InterruptedException
	{
		Program.mutex_vysetrenie.lock();
		
		Program.safePrintln(this.j + " lahko chory sa dostavil pred miestnost");
//		System.out.println(this.j + " lahko chory sa dostavil pred miestnost");
		
		while(Program.pritomni_t != 0 || Program.pritomni_l >= 2)
			Program.cond_l.await();
		
		Program.pritomni_l++;
		Program.safePrintln(this.j + " lahko chory bude vstupovat do miestnosti");
//		System.out.println(this.j + " lahko chory bude vstupovat do miestnosti");
		
		Program.mutex_vysetrenie.unlock();
		
		synchronized (Program.mutex_lahko_chori)
		{
			Program.safePrintln("vysetrenie " + this.j + " lahko choreho");
//			System.out.println("vysetrenie " + this.j + " lahko choreho");
			Thread.sleep(Program.SLEEP_LAHKO_CHORY);
			Program.count_lahko_chori++;
			Program.count_vsetci++;
		}
		
		Program.mutex_vysetrenie.lock();
		Program.pritomni_l--;
		Program.safePrintln("lahko chory " + this.j + " opustuje miestnost");
//		System.out.println("lahko chory " + this.j + " opustuje miestnost");
		Program.cond_t.signalAll();
		Program.cond_l.signalAll();
		Program.mutex_vysetrenie.unlock();
	}
	
	
	private void volno_lahko_chory() throws InterruptedException
	{
		Program.safePrintln("lahko chory pacient " + this.j + " je doma");
//		System.out.println("lahko chory pacient " + this.j + " je doma");
		Thread.sleep(Program.LAHKO_CHORY_DOMA);
	}

}
