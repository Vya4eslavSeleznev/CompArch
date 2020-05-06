package coursework;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;

public class UserInterface
{
  public UserInterface()
  {
    JFrame frame = new JFrame();
    frame.add(rootPanel);
    frame.setTitle("BMP converter");
    frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
    frame.setSize(900, 700);
    frame.setLocation(290, 50);
    frame.setVisible(true);

    comboBit.addItem("24-bit");
    comboBit.addItem("16-bit");
    comboBit.addItem("8-bit");
    comboBit.addItem("4-bit");
    comboBit.addItem("1-bit");
    comboBit.setSelectedItem("24-bit");
  }

  public void interfaceImplementation()
  {
    pictureLabel.setText("Here will be your picture");
    String pathToExe = "C:\\University\\Repositories\\CompArch\\coursework\\Debug\\coursework.exe";
    String pathToResultImage = "C:\\University\\Repositories\\CompArch\\GUI\\Result.bmp";

    File file = new File(pathToResultImage);
    file.delete();

    convertButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed (ActionEvent e)
      {
        String bit;
        String commandValue = (String) comboBit.getSelectedItem();
        String pathToImage = pathField.getText();
        pathToImage = pathToImage.trim();
        errorLabel.setText("");

        try
        {
          switch(commandValue)
          {
            case "24-bit":;
              BufferedImage myPicture24 = ImageIO.read(new File(pathToImage));
              ImageIcon image24 = new ImageIcon(myPicture24);
              pictureLabel.setText("");
              pictureLabel.setIcon(image24);
              break;

            case "16-bit":
              bit = "16";
              Process process16 = new ProcessBuilder(pathToExe, pathToImage, bit).start();
              process16.waitFor();
              BufferedImage myPicture16 = ImageIO.read(new File(pathToResultImage));
              ImageIcon image16 = new ImageIcon(myPicture16);
              pictureLabel.setText("");
              pictureLabel.setIcon(image16);
              break;

            case "8-bit":
              bit = "8";
              Process process8 = new ProcessBuilder(pathToExe, pathToImage, bit).start();
              process8.waitFor();
              BufferedImage myPicture8 = ImageIO.read(new File(pathToResultImage));
              ImageIcon image8 = new ImageIcon(myPicture8);
              pictureLabel.setText("");
              pictureLabel.setIcon(image8);
              break;

            case "4-bit":
              bit = "4";
              Process process4 = new ProcessBuilder(pathToExe, pathToImage, bit).start();
              process4.waitFor();
              BufferedImage myPicture4 = ImageIO.read(new File(pathToResultImage));
              ImageIcon image4 = new ImageIcon(myPicture4);
              pictureLabel.setText("");
              pictureLabel.setIcon(image4);
              break;

            case "1-bit":
              bit = "1";
              Process process1 = new ProcessBuilder(pathToExe, pathToImage, bit).start();
              process1.waitFor();
              BufferedImage myPicture1 = ImageIO.read(new File(pathToResultImage));
              ImageIcon image1 = new ImageIcon(myPicture1);
              pictureLabel.setText("");
              pictureLabel.setIcon(image1);
              break;
          }
        }
        catch(Exception err)
        {
          errorLabel.setText("Error");
        }
      }
    });
  }

  private JPanel rootPanel;
  private JButton convertButton;
  private JTextField pathField;
  private JComboBox<String> comboBit;
  private JLabel pictureLabel;
  private JLabel errorLabel;
}
