public class E : VisualCommanderExt.IExtension
{
	public void SetSite(EnvDTE80.DTE2 DTE, Microsoft.VisualStudio.Shell.Package package)
	{
		events = DTE.Events;
		windowEvents = events.WindowEvents;
		windowEvents.WindowActivated += OnWindowActivated;
		System.IServiceProvider serviceProvider = package as System.IServiceProvider;
		statusBar = serviceProvider.GetService(
			typeof(Microsoft.VisualStudio.Shell.Interop.SVsStatusbar)) as 
				Microsoft.VisualStudio.Shell.Interop.IVsStatusbar;
	}
	public void Close()
	{
		windowEvents.WindowActivated -= OnWindowActivated;
	}
	private void OnWindowActivated(EnvDTE.Window gotFocus, EnvDTE.Window lostFocus)
	{
		try
		{
			if (gotFocus.Document != null)
				Show(gotFocus.Document.FullName);
			else
				Show(gotFocus.Caption);
		}
		catch (System.Exception e)
		{
		}
	}
	private void Show(string s)
	{
		int frozen;
		statusBar.IsFrozen(out frozen);
		if (frozen == 0)
			statusBar.SetText(s);
	}
	private EnvDTE.Events events;
	private EnvDTE.WindowEvents windowEvents;
	private Microsoft.VisualStudio.Shell.Interop.IVsStatusbar statusBar;
}