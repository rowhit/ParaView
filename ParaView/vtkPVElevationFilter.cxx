/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPVElevationFilter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 1998-2000 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=========================================================================*/

#include "vtkPVElevationFilter.h"
#include "vtkKWApplication.h"
#include "vtkKWView.h"
#include "vtkKWRenderView.h"
#include "vtkPVComposite.h"

int vtkPVElevationFilterCommand(ClientData cd, Tcl_Interp *interp,
				int argc, char *argv[]);

vtkPVElevationFilter::vtkPVElevationFilter()
{
  this->CommandFunction = vtkPVElevationFilterCommand;
  
  this->LowPointLabel = vtkKWLabel::New();
  this->LowPointLabel->SetParent(this);
  this->LowPointFrame = vtkKWWidget::New();
  this->LowPointFrame->SetParent(this);
  this->LowPointXEntry = vtkKWEntry::New();
  this->LowPointXEntry->SetParent(this->LowPointFrame);
  this->LowPointYEntry = vtkKWEntry::New();
  this->LowPointYEntry->SetParent(this->LowPointFrame);
  this->LowPointZEntry = vtkKWEntry::New();
  this->LowPointZEntry->SetParent(this->LowPointFrame);

  this->HighPointLabel = vtkKWLabel::New();
  this->HighPointLabel->SetParent(this);
  this->HighPointFrame = vtkKWWidget::New();
  this->HighPointFrame->SetParent(this);
  this->HighPointXEntry = vtkKWEntry::New();
  this->HighPointXEntry->SetParent(this->HighPointFrame);
  this->HighPointYEntry = vtkKWEntry::New();
  this->HighPointYEntry->SetParent(this->HighPointFrame);
  this->HighPointZEntry = vtkKWEntry::New();
  this->HighPointZEntry->SetParent(this->HighPointFrame);

  this->RangeLabel = vtkKWLabel::New();
  this->RangeLabel->SetParent(this);
  this->RangeFrame = vtkKWWidget::New();
  this->RangeFrame->SetParent(this);
  this->RangeMinEntry = vtkKWEntry::New();
  this->RangeMinEntry->SetParent(this->RangeFrame);
  this->RangeMaxEntry = vtkKWEntry::New();
  this->RangeMaxEntry->SetParent(this->RangeFrame);

  this->LowPointXLabel = vtkKWLabel::New();
  this->LowPointXLabel->SetParent(this->LowPointFrame);
  this->LowPointYLabel = vtkKWLabel::New();
  this->LowPointYLabel->SetParent(this->LowPointFrame);
  this->LowPointZLabel = vtkKWLabel::New();
  this->LowPointZLabel->SetParent(this->LowPointFrame);
  this->HighPointXLabel = vtkKWLabel::New();
  this->HighPointXLabel->SetParent(this->HighPointFrame);
  this->HighPointYLabel = vtkKWLabel::New();
  this->HighPointYLabel->SetParent(this->HighPointFrame);
  this->HighPointZLabel = vtkKWLabel::New();
  this->HighPointZLabel->SetParent(this->HighPointFrame);
  this->RangeMinLabel = vtkKWLabel::New();
  this->RangeMinLabel->SetParent(this->RangeFrame);
  this->RangeMaxLabel = vtkKWLabel::New();
  this->RangeMaxLabel->SetParent(this->RangeFrame);
  
  this->Accept = vtkKWWidget::New();
  this->Accept->SetParent(this);
  
  this->Elevation = vtkElevationFilter::New();
}

vtkPVElevationFilter::~vtkPVElevationFilter()
{
  this->LowPointLabel->Delete();
  this->LowPointLabel = NULL;
  this->LowPointXEntry->Delete();
  this->LowPointXEntry = NULL;
  this->LowPointYEntry->Delete();
  this->LowPointYEntry = NULL;
  this->LowPointZEntry->Delete();
  this->LowPointZEntry = NULL;
  this->HighPointLabel->Delete();
  this->HighPointLabel = NULL;
  this->HighPointXEntry->Delete();
  this->HighPointXEntry = NULL;
  this->HighPointYEntry->Delete();
  this->HighPointYEntry = NULL;
  this->HighPointZEntry->Delete();
  this->HighPointZEntry = NULL;
  this->RangeLabel->Delete();
  this->RangeLabel = NULL;
  this->RangeMinEntry->Delete();
  this->RangeMinEntry = NULL;
  this->RangeMaxEntry->Delete();
  this->RangeMaxEntry = NULL;
  
  this->LowPointXLabel->Delete();
  this->LowPointXLabel = NULL;
  this->LowPointYLabel->Delete();
  this->LowPointYLabel = NULL;
  this->LowPointZLabel->Delete();
  this->LowPointZLabel = NULL;
  this->HighPointXLabel->Delete();
  this->HighPointXLabel = NULL;
  this->HighPointYLabel->Delete();
  this->HighPointYLabel = NULL;
  this->HighPointZLabel->Delete();
  this->HighPointZLabel = NULL;
  this->RangeMinLabel->Delete();
  this->RangeMinLabel = NULL;
  this->RangeMaxLabel->Delete();
  this->RangeMaxLabel = NULL;

  this->LowPointFrame->Delete();
  this->LowPointFrame = NULL;
  this->HighPointFrame->Delete();
  this->HighPointFrame = NULL;
  this->RangeFrame->Delete();
  this->RangeFrame = NULL;
  
  this->Accept->Delete();
  this->Accept = NULL;
  
  this->Elevation->Delete();
  this->Elevation = NULL;
}

vtkPVElevationFilter* vtkPVElevationFilter::New()
{
  return new vtkPVElevationFilter();
}

void vtkPVElevationFilter::Create(vtkKWApplication *app, char *args)
{ 
  float *low, *high, *range;
 
  // must set the application
  if (this->Application)
    {
    vtkErrorMacro("vtkPVElevationFilter already created");
    return;
    }
  this->SetApplication(app);
  
  // create the top level
  this->Script("frame %s %s", this->GetWidgetName(), args);
  
  low = this->GetElevation()->GetLowPoint();
  high = this->GetElevation()->GetHighPoint();
  range = this->GetElevation()->GetScalarRange();

  this->Accept->Create(this->Application, "button",
		       "-text Accept");
  this->Accept->SetCommand(this, "ElevationParameterChanged");

  this->LowPointLabel->Create(this->Application, "-pady 6");
  this->LowPointLabel->SetLabel("LowPoint");
  this->LowPointFrame->Create(this->Application, "frame", "-bd 0");
  this->LowPointXLabel->Create(this->Application, "-padx 4");
  this->LowPointXLabel->SetLabel("X:");
  this->LowPointXEntry->Create(this->Application, "-width 10");
  this->LowPointXEntry->SetValue(low[0], 2);
  this->Script("pack %s -side left -anchor w -expand no",
	       this->LowPointXLabel->GetWidgetName());
  this->Script("pack %s -side left -anchor w -expand yes",
	       this->LowPointXEntry->GetWidgetName());
  this->LowPointYLabel->Create(this->Application, "-padx 4");
  this->LowPointYLabel->SetLabel("Y:");
  this->LowPointYEntry->Create(this->Application, "-width 10");
  this->LowPointYEntry->SetValue(low[1], 2);
  this->Script("pack %s -side left -anchor w -expand no",
	       this->LowPointYLabel->GetWidgetName());
  this->Script("pack %s -side left -anchor w -expand yes",
	       this->LowPointYEntry->GetWidgetName());
  this->LowPointZLabel->Create(this->Application, "-padx 4");
  this->LowPointZLabel->SetLabel("Z:");
  this->LowPointZEntry->Create(this->Application, "-width 10");
  this->LowPointZEntry->SetValue(low[2], 2);
  this->Script("pack %s -side left -anchor w -expand no",
	       this->LowPointZLabel->GetWidgetName());
  this->Script("pack %s -side left -anchor w -expand yes",
	       this->LowPointZEntry->GetWidgetName());
  this->Script("pack %s %s %s",
	       this->Accept->GetWidgetName(),
	       this->LowPointLabel->GetWidgetName(),
	       this->LowPointFrame->GetWidgetName());
  
  this->HighPointFrame->Create(this->Application, "frame", "-bd 0");
  this->HighPointLabel->Create(this->Application, "-pady 6");
  this->HighPointLabel->SetLabel("High Point");
  this->HighPointXLabel->Create(this->Application, "-padx 4");
  this->HighPointXLabel->SetLabel("X:");
  this->HighPointXEntry->Create(this->Application, "-width 10");
  this->HighPointXEntry->SetValue(high[0], 2);
  this->Script("pack %s -side left -anchor w -expand no",
	       this->HighPointXLabel->GetWidgetName());
  this->Script("pack %s -side left -anchor w -expand yes",
	       this->HighPointXEntry->GetWidgetName());
  this->HighPointYLabel->Create(this->Application, "-padx 4");
  this->HighPointYLabel->SetLabel("Y:");
  this->HighPointYEntry->Create(this->Application, "-width 10");
  this->HighPointYEntry->SetValue(high[1], 2);
  this->Script("pack %s -side left -anchor w -expand no",
	       this->HighPointYLabel->GetWidgetName());
  this->Script("pack %s -side left -anchor w -expand yes",
	       this->HighPointYEntry->GetWidgetName());
  this->HighPointZLabel->Create(this->Application, "-padx 4");
  this->HighPointZLabel->SetLabel("Z:");
  this->HighPointZEntry->Create(this->Application, "-width 10");
  this->HighPointZEntry->SetValue(high[2], 2);
  this->Script("pack %s -side left -anchor w -expand no",
	       this->HighPointZLabel->GetWidgetName());
  this->Script("pack %s -side left -anchor w -expand yes",
	       this->HighPointZEntry->GetWidgetName());
  this->Script("pack %s %s",
	       this->HighPointLabel->GetWidgetName(),
	       this->HighPointFrame->GetWidgetName());
  
  this->RangeFrame->Create(this->Application, "frame", "-bd 0");
  this->RangeLabel->Create(this->Application, "-pady 6");
  this->RangeLabel->SetLabel("Scalar Range");
  this->RangeMinLabel->Create(this->Application, "-padx 4");
  this->RangeMinLabel->SetLabel("Min.:");
  this->RangeMinEntry->Create(this->Application, "-width 10");
  this->RangeMinEntry->SetValue(range[0], 2);
  this->Script("pack %s -side left -anchor w -expand no",
	       this->RangeMinLabel->GetWidgetName());
  this->Script("pack %s -side left -anchor w -expand yes",
	       this->RangeMinEntry->GetWidgetName());
  this->RangeMaxLabel->Create(this->Application, "-padx 4");
  this->RangeMaxLabel->SetLabel("Max.:");
  this->RangeMaxEntry->Create(this->Application, "-width 10");
  this->RangeMaxEntry->SetValue(range[1], 2);
  this->Script("pack %s -side left -anchor w -expand no",
	       this->RangeMaxLabel->GetWidgetName());
  this->Script("pack %s -side left -anchor w -expand yes",
	       this->RangeMaxEntry->GetWidgetName());
  this->Script("pack %s %s",
	       this->RangeLabel->GetWidgetName(),
	       this->RangeFrame->GetWidgetName());
}

void vtkPVElevationFilter::ElevationParameterChanged()
{
  float low[3], high[3], range[2];

  low[0] = this->LowPointXEntry->GetValueAsFloat();
  low[1] = this->LowPointYEntry->GetValueAsFloat();
  low[2] = this->LowPointZEntry->GetValueAsFloat();
  high[0] = this->HighPointXEntry->GetValueAsFloat();
  high[1] = this->HighPointYEntry->GetValueAsFloat();
  high[2] = this->HighPointZEntry->GetValueAsFloat();
  range[0] = this->RangeMinEntry->GetValueAsFloat();
  range[1] = this->RangeMaxEntry->GetValueAsFloat();
  
  this->GetElevation()->SetLowPoint(low);
  this->GetElevation()->SetHighPoint(high);
  this->GetElevation()->SetScalarRange(range);
  
  this->Composite->GetView()->Render();
}
