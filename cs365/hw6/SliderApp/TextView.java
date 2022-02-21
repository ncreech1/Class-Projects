package SliderApp;
import javax.swing.JLabel;

/* Nicholas Creech
 * CS365 HW6
 * 03/24/21
 */ 

class TextView extends JLabel implements AppView
{
	private AppModel model;

	public TextView(AppModel parent)
	{
		model = parent;
		model.addObserver(this);
		setText("Value: " + Integer.toString(model.getSliderVal()));
	}

	public void update(int oldVal)
	{
		setText("Value: " + Integer.toString(model.getSliderVal()));
	}
}
