/* 
 * Copyright (c) 2008, 2013, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

using System;

using MySQL.Grt;
using MySQL.Grt.Db;

namespace MySQL.GUI.Workbench.Plugins
{
  public partial class DbMysqlRoutineEditor : ObjectEditorPlugin
  {
    #region Member Variables

    private MySQLRoutineEditorBE routineEditorBE { get { return Backend as MySQLRoutineEditorBE; } } 
    private DbObjectEditorPages dbObjectEditorPages;

    #endregion

    #region Constructors

    public DbMysqlRoutineEditor(GrtManager manager, GrtValue value)
      : base(manager)
    {
      InitializeComponent();
      ReinitWithArguments(value);
      if (IsEditingLiveObject)
      {
        mainTabControl.TabPages.Remove(commentTabPage);
        AdjustEditModeControls(mainTabControl);
      }
    }

    #endregion

    #region ObjectEditorPlugin Overrides

    public override bool ReinitWithArguments(GrtValue value)
    {
      InitializingControls = true;

      try
      {
        Backend = new MySQLRoutineEditorBE(GrtManager, value);
        dbObjectEditorPages = new DbObjectEditorPages(GrtManager, routineEditorBE);
        SetupEditorOnHost(panel2, true);
        routineEditorBE.load_routine_sql();
        InitFormData();
        RefreshFormData();

        Backend.reset_editor_undo_stack();
      }
      finally
      {
        InitializingControls = false;
      }

      Invalidate();

      return true;
    }

    #endregion

    #region Form implementation

    private void DbMysqlRoutineEditor_Load(object sender, EventArgs e)
    {
      ActivateEditor();
    }

    protected void InitFormData()
    {
      // Add privileges tab sheet if we are not editing a live object.
      if (mainTabControl.TabPages.ContainsKey(dbObjectEditorPages.PrivilegesTabPage.Name))
      {
        if (!IsEditingLiveObject)
          mainTabControl.TabPages.Remove(dbObjectEditorPages.PrivilegesTabPage);
      }
      else
      {
        if (!IsEditingLiveObject)
          mainTabControl.TabPages.Add(dbObjectEditorPages.PrivilegesTabPage);
      }
    }

    protected override void RefreshFormData()
    {
      base.RefreshFormData();

      nameTextBox.Text = routineEditorBE.get_name();
      TabText = routineEditorBE.get_title();
      if (!IsEditingLiveObject)
        commentTextBox.Text = routineEditorBE.get_comment();
      routineEditorBE.load_routine_sql();
    }

    #endregion
    
    #region Event Handling

    private void commentTextBox_Leave(object sender, EventArgs e)
    {
      if (!IsEditingLiveObject)
        routineEditorBE.set_comment(commentTextBox.Text);
    }

    #endregion

  }
}