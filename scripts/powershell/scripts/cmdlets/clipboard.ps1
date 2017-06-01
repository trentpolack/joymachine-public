# From: https://stackoverflow.com/a/1573295

function Get-ClipBoard
{
    Add-Type -AssemblyName System.Windows.Forms
    $tb = New-Object System.Windows.Forms.TextBox
    $tb.Multiline = $true
    $tb.Paste()
    $tb.Text
}


function Set-ClipBoard( )
{
    Param(
      [Parameter(ValueFromPipeline=$true)]
      [string] $text
    )

    Add-Type -AssemblyName System.Windows.Forms
    $tb = New-Object System.Windows.Forms.TextBox
    $tb.Multiline = $true
    $tb.Text = $text
    $tb.SelectAll()
    $tb.Copy()
}