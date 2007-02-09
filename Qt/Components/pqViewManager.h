/*=========================================================================

   Program: ParaView
   Module:    pqViewManager.h

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.1. 

   See License_v1.1.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __pqViewManager_h
#define __pqViewManager_h

#include "pqMultiView.h"

class pqGenericViewModule;
class pqMultiView;
class pqMultiViewFrame;
class pqServer;
class vtkPVXMLElement;
class vtkSMStateLoader;

/// This class manages all view windows. View windows occupy the central
/// area in the application are all layed out using split windows. This 
/// split window management is take care by the superclass pqMultiView.
/// This class controls creation/deletion of view modules of the frames 
/// created/deleted by superclass. Note that all view modules,
/// includes 3D render modules, plot views etc are organized by this class.
class PQCOMPONENTS_EXPORT pqViewManager : public pqMultiView 
{
  Q_OBJECT
public:
  pqViewManager(QWidget* parent=NULL);
  virtual ~pqViewManager();

  /// returns the active view module.
  pqGenericViewModule* getActiveViewModule() const;

  /// Save the state of the view window manager.
  void saveState(vtkPVXMLElement* root);

  /// Loads the state for the view window manager.
  bool loadState(vtkPVXMLElement* rwRoot, vtkSMStateLoader* loader);

  /// This option is used for testing. When size.isEmpty() is true,
  /// it resets the maximum bounds on the view windows.
  /// This is useful when running tests, so that we are guranteed that 
  ///  the view size is fixed.
  void setMaxViewWindowSize(const QSize& size);

  /// Given a view module, get the frame in which the view is contained,
  /// if any.
  pqMultiViewFrame* getFrame(pqGenericViewModule* view) const;
signals:
  /// Fired when the active view module changes.
  void activeViewModuleChanged(pqGenericViewModule*);

  // Fired when the user pressed the lookmark button for one of the views
  void createLookmark(pqGenericViewModule*);

private slots:
  /// This will create a view module to fill the frame.
  /// the render window is created on the active server
  /// which must be set by the application.
  void onFrameAdded(pqMultiViewFrame* frame);
  void onFrameRemoved(pqMultiViewFrame* frame);

  /// When ever a new view module is noticed, the active 
  /// frame is split and the view module is shown in the new 
  /// split frame.
  void onViewModuleAdded(pqGenericViewModule* rm);

  /// When ever a view module is removed, we also close
  /// the frame containing the view module.
  void onViewModuleRemoved(pqGenericViewModule* rm);

  /// Called when a frame becomes active. It will
  /// inactivate all other frames and trigger activeViewModuleChanged().
  void onActivate(QWidget* obj);

  /// Called when user requests conversion of view type.
  void onConvertToTriggered(QAction* action);

  /// Called before context menu is shown for the frame.
  /// We update menu enable state depending on view type.
  void onFrameContextMenuRequested(QWidget*);

  /// Slots to manage drag/drop of frames.
  void frameDragStart(pqMultiViewFrame*);
  void frameDragEnter(pqMultiViewFrame*,QDragEnterEvent*);
  void frameDragMove(pqMultiViewFrame*,QDragMoveEvent*);
  void frameDrop(pqMultiViewFrame*,QDropEvent*);

public slots:
  /// Set the active server. This must be called whenever
  /// active server changes. The active server is used to 
  /// determine the server on whcih to create new view modules
  /// when a frame a split.
  void setActiveServer(pqServer* server);

  // Emits a signal to let listeners know that a lookmark 
  // needs to be created of the active view
  void onLookmarkButtonPressed();

protected:
  /// Event filter callback.
  bool eventFilter(QObject* caller, QEvent* e);

  /// This method will either assign an empty frame
  /// to the view module or split the active view
  /// (if any, otherwise splits the first view)
  /// to create a new frame and add the view
  /// to it.
  void assignFrame(pqGenericViewModule* view);

  // Create/disconnects appropriate signal/slot connections between
  // the view and the frame.
  void connect(pqMultiViewFrame* frame, pqGenericViewModule* view);
  void disconnect(pqMultiViewFrame* frame, pqGenericViewModule* view);

  /// Hiding superclasses loadState. Don't use this API
  /// since it is not aware of the loader which gives us 
  /// the render modules to put in the window.
  virtual void loadState(vtkPVXMLElement* root)
    {
    this->pqMultiView::loadState(root);
    }

  /// Update the GUISize/WindowPosition properties
  /// on all view modules.
  void updateViewModulePositions();

  /// Updates the context menu.
  void updateConversionActions(pqMultiViewFrame* frame);
private:
  pqViewManager(pqViewManager&); // Not implemented.
  void operator=(const pqViewManager&); // Not implemented.


  class pqInternals;
  pqInternals* Internal;
};

#endif

