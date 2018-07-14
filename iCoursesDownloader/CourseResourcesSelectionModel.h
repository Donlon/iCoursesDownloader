#pragma once
#include "qitemselectionmodel.h"
#include "CourseResourcesModel.h"

class CourseResourcesSelectionModel :
	public QItemSelectionModel
{
public:
    explicit CourseResourcesSelectionModel(CourseResourcesModel *model = 0): QItemSelectionModel(model), model(model), treeRoot(model->treeRoot){;};
    explicit CourseResourcesSelectionModel(CourseResourcesModel *model, QObject *parent): QItemSelectionModel(model, parent){;};
	~CourseResourcesSelectionModel(void);
    Q_INVOKABLE bool isSelected(const QModelIndex& index) const;
public Q_SLOTS:
	virtual void setCurrentIndex(const QModelIndex& index, QItemSelectionModel::SelectionFlags command) Q_DECL_OVERRIDE;
	virtual void select(const QModelIndex& index, QItemSelectionModel::SelectionFlags command) Q_DECL_OVERRIDE;
	virtual void select(const QItemSelection& selection, QItemSelectionModel::SelectionFlags command) Q_DECL_OVERRIDE;
	virtual void clear() override;
	virtual void reset() override;

	virtual void clearCurrentIndex() override;
private:
	CourseResourcesModel *model;
	CourseResourcesTree *treeRoot;
};

