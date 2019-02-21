import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Set;

/**
 * Class for a miner.
 */
public class Miner extends Thread {
	/**
	 * Creates a {@code Miner} object.
	 * 
	 * @param hashCount
	 *            number of times that a miner repeats the hash operation when
	 *            solving a puzzle.
	 * @param solved
	 *            set containing the IDs of the solved rooms
	 * @param channel
	 *            communication channel between the miners and the wizards
	 */
	Integer hashCount;
	Set<Integer> solved;
	CommunicationChannel channel;
	
	public Miner(Integer hashCount, Set<Integer> solved, CommunicationChannel channel) {
		this.hashCount = hashCount;
		this.solved = solved;
		this.channel = channel;
	}
	
    private static String encryptThisString(String input) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            byte[] messageDigest = md.digest(input.getBytes(StandardCharsets.UTF_8));

            // convert to string
            StringBuffer hexString = new StringBuffer();
            for (int i = 0; i < messageDigest.length; i++) {
            String hex = Integer.toHexString(0xff & messageDigest[i]);
            if(hex.length() == 1) hexString.append('0');
                hexString.append(hex);
            }
            return hexString.toString();

        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        }
    }

	@Override
	public void run() {
		Message wizardMessage;
		Integer parent, neighbor;
		String hash;
		
		while(true) {
			// Mereu vreau sa obtin controlul pentru 2 citiri sau un null
			try {
				CommunicationChannel.readTwoElems.acquire();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			wizardMessage = channel.getMessageWizardChannel();
			
			//Cazuri de baza
			if(wizardMessage == null) {
				CommunicationChannel.readTwoElems.release();
				continue;
			} else if(wizardMessage.getData().equals("END")) {
				CommunicationChannel.readTwoElems.release();
				continue;
			} else if(wizardMessage.getData().equals("EXIT")) {
				CommunicationChannel.readTwoElems.release();
				return;
			//Cazul favorabil
			} else {
				//initializare + noua citire
				parent = wizardMessage.getCurrentRoom();
				wizardMessage = channel.getMessageWizardChannel();
				CommunicationChannel.readTwoElems.release();
				hash = wizardMessage.getData();
				neighbor = wizardMessage.getCurrentRoom();
				
				//daca e rezolvat atunci sar peste
				synchronized(channel) {
					if(solved.contains(neighbor) == true) {
						continue;
					} else {
						solved.add(neighbor);
					}
				}
				
				//decriptez
				for(int i = 0; i < hashCount; i++)
					hash = encryptThisString(hash);
				
				//trimit
				channel.putMessageMinerChannel(new Message(parent, neighbor, hash));
			}
			
			
		}
		
	}
}
