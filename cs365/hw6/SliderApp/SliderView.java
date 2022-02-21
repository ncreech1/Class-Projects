package SliderApp;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JSlider;
import javax.swing.BoxLayout;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;
import javax.swing.JOptionPane;

/* Nicholas Creech
 * CS365 HW6
 * 03/24/21
 */ 

class SliderView extends JPanel implements AppView
{
	private AppModel model;
	private JButton decButton;
	private JButton incButton;
	private JSlider slider;

	public SliderView(AppModel parent)
	{
		model = parent;
		model.addObserver(this);
		
		setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
		decButton = new	JButton("Decrement");
		incButton = new JButton("Increment");
		slider = new JSlider(model.getMinVal(), model.getMaxVal(), model.getSliderVal());

		//Add decrement button listener
		decButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e) { decButtonAction(e); }
		});

		//Add increment button listener
		incButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e) { incButtonAction(e); }
		});

		//Add slider listener
		slider.addChangeListener(new ChangeListener()
		{
			public void stateChanged(ChangeEvent e) { sliderAction(e); }
		});

		add(decButton);
		add(slider);
		add(incButton);
	}

	public void sliderAction(ChangeEvent e)
	{
		model.setSliderVal(slider.getValue());
	}

	public void decButtonAction(ActionEvent e)
	{
		//Only decrement if not at min value
		if(model.getSliderVal() == model.getMinVal())
			JOptionPane.showMessageDialog(null, "Cannot further decrement the value", "Error", JOptionPane.ERROR_MESSAGE);

		else
		{
			if(model.getSliderVal() - model.getIncrement() < model.getMinVal())
				model.setSliderVal(model.getMinVal());
			else 
				model.setSliderVal(model.getSliderVal() - model.getIncrement());
		}
	}

	public void incButtonAction(ActionEvent e)
	{
		//Only increment if not at max value
		if(model.getSliderVal() == model.getMaxVal())
			JOptionPane.showMessageDialog(null, "Cannot further increment the value", "Error", JOptionPane.ERROR_MESSAGE);

		else
		{
			if(model.getSliderVal() + model.getIncrement() > model.getMaxVal())
				model.setSliderVal(model.getMaxVal());
			else 
				model.setSliderVal(model.getSliderVal() + model.getIncrement());
		}
	}

	public void update(int oldVal)
	{
		slider.setValue(model.getSliderVal());
	}
}
