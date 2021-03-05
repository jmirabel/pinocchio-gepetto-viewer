#pragma once

#include <QItemDelegate>

#include "plugin.hh"

namespace gepetto {
namespace pinocchio {

class JointItemDelegate : public QItemDelegate
{
  Q_OBJECT

  public:
    JointItemDelegate (PinocchioPlugin* plugin, gepetto::gui::MainWindow* parent);

    void updateTypeRole (JointTreeItem::ItemType& type) const;

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

  private:
    gepetto::gui::MainWindow* main_;
    HppWidgetsPlugin* plugin_;
    QPushButton* forceIntegrator_;
};

} // namespace pinocchio
} // namespace gepetto
