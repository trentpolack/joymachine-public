Imports EnvDTE
Imports EnvDTE80
Imports Microsoft.VisualStudio.Shell
Imports System
Public Class E
	Implements VisualCommanderExt.IExtension
	Sub SetSite(DTE_ As DTE2, package As Package) Implements VisualCommanderExt.IExtension.SetSite
		DTE = DTE_
		events = DTE.Events
		buildEvents = events.BuildEvents
		AddHandler buildEvents.OnBuildBegin, AddressOf OnBuildBegin
		AddHandler buildEvents.OnBuildDone, AddressOf OnBuildDone
	End Sub
	Sub Close() Implements VisualCommanderExt.IExtension.Close
		RemoveHandler buildEvents.OnBuildBegin, AddressOf OnBuildBegin
		RemoveHandler buildEvents.OnBuildDone, AddressOf OnBuildDone
	End Sub
	Private Sub OnBuildBegin(ByVal Scope As EnvDTE.vsBuildScope, ByVal Action As vsBuildAction)
		timerStart = DateTime.Now

		Dim window As Window = DTE.Windows.Item(EnvDTE.Constants.vsWindowKindOutput)
		Dim output As OutputWindow = CType(window.Object, OutputWindow)
		For Each pane As OutputWindowPane In output.OutputWindowPanes
			If (pane.Name = "Build") Then
				pane.Activate()
				pane.OutputString("Build Started: " & DateTime.Now.ToLocalTime().ToString() & Environment.NewLine)
			End If
		Next

		window.Activate()
	End Sub
	Private Sub OnBuildDone(ByVal Scope As EnvDTE.vsBuildScope, ByVal Action As vsBuildAction)
		Dim ow As OutputWindow = DTE.Windows.Item(EnvDTE.Constants.vsWindowKindOutput).Object
		Dim build As OutputWindowPane = ow.OutputWindowPanes.Item("Build")

		build.Activate()

		Dim completed As String = "Build Completed: " & DateTime.Now.ToLocalTime().ToString() & Environment.NewLine & "Build Time: " & DateTime.Now.Subtract(timerStart).ToString("hh\:mm\:ss") & Environment.NewLine & ""
		build.OutputString(completed)
		ow.Activate()
	End Sub
	Private DTE As EnvDTE80.DTE2
	Private events As EnvDTE.Events
	Private buildEvents As EnvDTE.BuildEvents
	Private timerStart As DateTime
End Class