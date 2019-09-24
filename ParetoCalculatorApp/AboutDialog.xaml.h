//
// AboutDialog.xaml.h
// Declaration of the AboutDialog class
//

#pragma once

#include "AboutDialog.g.h"

namespace ParetoCalculatorApp
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AboutDialog sealed
	{
	public:
		AboutDialog();
	private:
		void ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
		void ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
	};
}
