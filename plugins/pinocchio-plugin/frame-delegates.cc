#include "frame-delegates.hh"

namespace gepetto {
namespace pinocchio {

JointItemDelegate::JointItemDelegate(QPushButton *forceVelocity, HppWidgetsPlugin *plugin, gepetto::gui::MainWindow *parent)
  : QItemDelegate (parent), main_ (parent),
  plugin_ (plugin),
  forceIntegrator_ (forceVelocity)
{}

void JointItemDelegate::updateTypeRole (JointTreeItem::ItemType& type) const
{
  if (forceIntegrator_ && forceIntegrator_->isChecked () && (
        type == JointTreeItem::UnboundedValueType
        || type == JointTreeItem::BoundedValueType
        ))
    type = JointTreeItem::IntegratorType;
}

QWidget *JointItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
  JointTreeItem::ItemType type = (JointTreeItem::ItemType)index.data(JointTreeItem::TypeRole).toInt();
  updateTypeRole(type);
  const QStandardItemModel* m = static_cast <const QStandardItemModel*> (index.model());
  const JointTreeItem* ji = dynamic_cast <const JointTreeItem*> (m->itemFromIndex(index)->parent());
  switch (type) {
    case JointTreeItem::SkipType:
      return 0;
    case JointTreeItem::IntegratorType:
      assert (ji);
      return new IntegratorWheel (Qt::Horizontal, plugin_, parent, main_,
          ji, std::min ( (ULong)index.data(JointTreeItem::IndexRole).toInt(),
                         ji->numberDof()-1)
          );
    case JointTreeItem::BoundedValueType:
      assert (ji);
      return new SliderBoundedJoint (Qt::Horizontal, plugin_, parent,
          main_, ji, index.data(JointTreeItem::IndexRole).toInt());
    case JointTreeItem::UnboundedValueType:
    case JointTreeItem::BoundType: {
                                     QDoubleSpinBox* spinbox = new QDoubleSpinBox (parent);
                                     spinbox->setMinimum(-DBL_MAX);
                                     spinbox->setMaximum(DBL_MAX);
                                     spinbox->setSingleStep(0.01);
                                     spinbox->setDecimals(3);
                                     return spinbox;
                                   }
    default:
                                   return 0;
  }
}

void JointItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  JointTreeItem::ItemType type = (JointTreeItem::ItemType)index.data(JointTreeItem::TypeRole).toInt();
  updateTypeRole(type);
  float q = index.data(Qt::EditRole).toFloat ();
  switch (type) {
    case JointTreeItem::SkipType:
      return;
    case JointTreeItem::IntegratorType:
      return;
    case JointTreeItem::BoundedValueType:
      return;
    case JointTreeItem::UnboundedValueType:
    case JointTreeItem::BoundType:{
                                    QDoubleSpinBox* spinbox = static_cast <QDoubleSpinBox*> (editor);
                                    spinbox->setValue(q);
                                    break;
                                  }
    default:
                                  break;
  }
}

void JointItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
    const QModelIndex &index) const
{
  JointTreeItem::ItemType type = (JointTreeItem::ItemType)index.data(JointTreeItem::TypeRole).toInt();
  updateTypeRole(type);
  QStandardItemModel* m = static_cast <QStandardItemModel*> (model);
  JointTreeItem* ji = dynamic_cast <JointTreeItem*> (m->itemFromIndex(index)->parent());
  double q;
  switch (type) {
    case JointTreeItem::SkipType:
      return;
    case JointTreeItem::IntegratorType:
      return;
    case JointTreeItem::BoundedValueType: {
                                            SliderBoundedJoint* slider = static_cast <SliderBoundedJoint*> (editor);
                                            q = slider->getValue();
                                            break;
                                          }
    case JointTreeItem::UnboundedValueType:
    case JointTreeItem::BoundType: {
                                     QDoubleSpinBox* spinbox = static_cast <QDoubleSpinBox*> (editor);
                                     q = spinbox->value();
                                     break;
                                   }
    default:
                                   return;
  }
  model->setData(index, q, Qt::EditRole);
  assert (ji);
  ULong idxCfg = (ULong)index.data(JointTreeItem::IndexRole).toInt();
  switch (type) {
    case JointTreeItem::BoundedValueType:
      return;
    case JointTreeItem::UnboundedValueType:
      plugin_->currentConfig()[ji->rankInConfig()+idxCfg] = q;
      break;
    case JointTreeItem::BoundType:
      plugin_->client()->robot()->setJointBounds (ji->name().c_str(), ji->bounds());
      ji->updateTypeRole();
      break;
    default:
      break;
  }
  main_->requestApplyCurrentConfiguration();
}

void JointItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
  editor->setGeometry(option.rect);
}

} // namespace pinocchio
} // namespace gepetto
