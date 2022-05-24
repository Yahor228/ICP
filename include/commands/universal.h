/**
 * @file delete_class.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration of class for remove a class in CD
 */


#pragma once
#include <QUndoCommand>



class UniversalCommand : public QUndoCommand
{
public:
	UniversalCommand  ();
	~UniversalCommand  ();

private:

};