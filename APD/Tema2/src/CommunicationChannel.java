import java.util.ArrayList;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Class that implements the channel used by wizards and miners to communicate.
 */
public class CommunicationChannel {
	/**
	 * Creates a {@code CommunicationChannel} object.
	 */
	ArrayList<Message> wizardsMessage = new ArrayList<Message>();
	ArrayList<Message> minersMessage = new ArrayList<Message>();
	
	/*  Drept citire pentru mineri si vrajitori  */
	Semaphore minersRead = new Semaphore(0);
	Semaphore wizardsRead = new Semaphore(0);
	
	/*  putMessageWizardChannel  */
	Semaphore letWrite = new Semaphore(0);
	AtomicBoolean valid = new AtomicBoolean(false);
	
	/*  getMessageWizardChannel  */
	AtomicInteger indexMinersRead = new AtomicInteger(0);
	
	/*  getMessageMinerChannel  */
	AtomicInteger indexWizardsRead = new AtomicInteger(0);
	
	/*  Miner.java  */
	public static Semaphore readTwoElems = new Semaphore(1);

	public CommunicationChannel() {
	}

	/**
	 * Puts a message on the miner channel (i.e., where miners write to and wizards
	 * read from).
	 *
	 * @param message
	 *            message to be put on the channel
	 */
	public synchronized void putMessageMinerChannel(Message message) {
		minersMessage.add(message);
		wizardsRead.release();
	}

	/**
	 * Gets a message from the miner channel (i.e., where miners write to and
	 * wizards read from).
	 *
	 * @return message from the miner channel
	 */
	public Message getMessageMinerChannel() {
		try {
			wizardsRead.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		Message m = null;
		synchronized(minersMessage) {
			m = minersMessage.get(indexWizardsRead.getAndIncrement());
		}
		
		//Ocup doar pentru un Wizard
		try {
			letWrite.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		return m;
	}

	/**
	 * Puts a message on the wizard channel (i.e., where wizards write to and miners
	 * read from).
	 *
	 * @param message
	 *            message to be put on the channel
	 */
	public void putMessageWizardChannel(Message message){
		// Caz de baza
		if(message.getData().equals("NO_PARENT")) {
			try {
				letWrite.release();
				letWrite.acquire();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		// Un singur thread poate pune
		wizardsMessage.add(message);
		
		// Daca a ajuns la finalul transmisiei
		if(message.getData().equals("END")) {
			synchronized (wizardsMessage) {
				if(letWrite.availablePermits() == 0) {
					letWrite.release();
				}
			}
		}

		minersRead.release();
	}

	/**
	 * Gets a message from the wizard channel (i.e., where wizards write to and
	 * miners read from).
	 *
	 * @return message from the miner channel
	 */
	public Message getMessageWizardChannel() {

		try {
			minersRead.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		Message m = null;
		synchronized (wizardsMessage) {
			m = wizardsMessage.get(indexMinersRead.getAndIncrement());
		}
		return m;
	}
}
