package SliderApp;
import java.util.ArrayList;
import java.util.List;

/* Nicholas Creech
 * CS365 HW6
 * 03/24/21
 */

class AppModel
{
	List<AppView> observers = new ArrayList<AppView>();
	int sliderVal, maxVal, minVal, increment;

	int getSliderVal() { return sliderVal; }
	int getMaxVal() { return maxVal; }
	int getMinVal() { return minVal; }
	int getIncrement() { return increment; }

	public AppModel(int minVal, int maxVal, int startVal, int increment)
	{
		this.minVal = minVal;
		this.maxVal = maxVal;
		this.sliderVal = startVal;
		this.increment = increment;
	}

	//Sets the model slider state value and updates observers 
	void setSliderVal(int newVal)
	{	
		int oldVal = sliderVal;
		sliderVal = newVal;

		for(AppView view : observers)
			view.update(oldVal);
	}

	//Adds a new observer to the model
	void addObserver(AppView view) { observers.add(view); }
}
