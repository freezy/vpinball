#pragma once
#ifndef __VECTOR_H__
#define __VECTOR_H__

const int START_SIZE=10;
const int GROW_SIZE=10;

class VectorVoid
{
protected:
	int		m_cMax;		// Number of elements allocated
	int		m_cSize;	// Last element used
	void 	**m_rg;		// Data buffer

public:

	inline VectorVoid()
		{
		m_cSize = 0;
		m_cMax = 0;
		m_rg = NULL;
		SetSize(8);
		}

	inline VectorVoid(const int cSize)
		{
		m_cSize = 0;
		m_rg = (void **)malloc(sizeof(void *) * cSize);
		m_cMax = (!m_rg) ? 0 : cSize;
		}

	inline VectorVoid(const VectorVoid * const pvector)
		{		
		m_rg = (void **)malloc(sizeof(void *) * m_cMax);

		if (!m_rg)
			{
			m_cMax = 0;
			m_cSize = 0;

			return;
			}

		m_cMax = pvector->m_cMax;
		m_cSize = pvector->m_cSize;

		for (int i=0;i<m_cSize;i++)	// We need this for smart pointers - they need to be ref counted
			{
			m_rg[i] = pvector->m_rg[i];
			}
		}

	inline ~VectorVoid()
		{
		if (m_rg)
			free(m_rg);
		}
		
	// essentially a debug-only method to tell the Vector to toss its heap mem
	inline void Reset()
		{
		if (m_rg)
			{
			free(m_rg);
			m_rg = NULL;
			m_cSize = 0;
			m_cMax = 0;
			}
		}

	inline void Empty()
		{
		if (m_rg)
			free(m_rg);
		m_cSize = 0;
		m_rg = (void **)malloc(sizeof(void *) * START_SIZE);
		m_cMax = (!m_rg) ? 0 : START_SIZE;  // if !m_rg, we're actually OOM, but this will get caught the next time anything gets added to the Vector
		}

	inline int Size() const
		{
		return m_cSize;
		}

	inline int Capacity() const
		{
		return m_cMax;
		}
		
	inline void *ElementAt(const int iItem) const
		{
		return m_rg[iItem];
		}

	inline void *GetArray() const
		{
		return m_rg;
		}

	inline BOOL SetSize(const int i)
		{
		if (i > m_cMax)
			{		
			void **m_rgNew;

			if (m_rg)
				{
				m_rgNew = (void **)realloc((void *)m_rg, sizeof(void *) * (i));
				}
			else
				{
				m_rgNew = (void **)malloc(sizeof(void *) * i);
				}

			if (m_rgNew == NULL)
				{
				return fFalse;
				}

			m_rg = m_rgNew;
			m_cMax = i;
			}

		return fTrue;
		}

	inline BOOL Extend(const int cNewSize)
		{
		if (cNewSize > m_cSize)
			{
			if (!SetSize(cNewSize))
				return FALSE;

			memset(&m_rg[m_cSize], 0, sizeof(void *) * (cNewSize - m_cSize));
			m_cSize = cNewSize;
			}
		return fTrue;
		}

	inline BOOL Clone(VectorVoid * const pvector) const
		{
		if (m_rg)
			{
			pvector->Reset();
			if ((pvector->m_rg = (void **)malloc(sizeof(void *) * m_cMax)) == NULL)
				{
				pvector->m_cMax = 0;
				pvector->m_cSize = 0;

				return fFalse;  // OOM
				}
			}
		else
			{
			pvector->m_rg = NULL;
			}
		
		pvector->m_cMax = m_cMax;
		pvector->m_cSize = m_cSize;
			
		for (int i=0;i<m_cSize;i++)	// We need this for smart pointers - they need to be ref counted
			{
			pvector->m_rg[i] = m_rg[i];
			}
			
		return fTrue;
		}

	inline int LastIndexOf(const void * const pT) const
		{
		if (!m_rg)
			return -1;

		int i;
		for (i = m_cSize-1; i >= 0; i--)
			if (m_rg[i] == pT)
				break;

		return i;
		}

	inline int IndexOf(const void * const pvItem) const
		{
		if (m_rg == NULL)
			return -1;

		for (int i=0; i < m_cSize; i++)
			if (pvItem == m_rg[i])
				return i;

		return -1;
		}

	// returns current size of the Vector, or -1 on failure (OOM)
	inline int AddElement(void * const pItem)
		{
		if(m_cSize == m_cMax)
			{
			if (!SetSize(m_cSize*2))
				return -1;
			}

		m_rg[m_cSize++] = pItem;

		return (m_cSize - 1);
		}

	// fTrue for success, fFalse if failure (OOM)
	inline BOOL InsertElementAt(void * const pItem, const int iPos)
		{					
		if(m_cSize == m_cMax)
			{		
			if (!SetSize(m_cSize+GROW_SIZE))
				return fFalse;
			}

		if (m_cSize != iPos)
			{
			memcpy(m_rg+iPos+1, m_rg+iPos, sizeof(void *) * (m_cSize-iPos));
			}

		m_rg[iPos] = pItem;
		m_cSize++;
		return fTrue;
		}

	inline void ReplaceElementAt(void * const pItem, const int iPos)
		{
		if (!m_rg)
			return;

		m_rg[iPos] = pItem;
		}

	inline void RemoveElementAt(const int iItem)
		{
		if (!m_rg)
			return;

		memcpy(m_rg+iItem, m_rg+iItem+1, sizeof(void *) * (m_cSize-iItem-1));
		m_cSize--;
		}

	inline void RemoveElement(void * const pvItem)
		{
		if (!m_rg)
			return;

		const int i = IndexOf(pvItem);
		if (i >= 0)
			RemoveElementAt(i);
		}

	inline void RemoveAllElements()
		{
		if (!m_rg)
			return;
		//for (int i=0;i<m_cSize;i++) 	// We need this for smart pointers - they need to be ref counted
			//{
			//m_rg[i] = NULL;
			//}
		m_cSize = 0;
		}

    // LIFO stack support
    inline int Push(void * const pItem)
    {
        return AddElement(pItem);
    }

    inline BOOL Pop(void ** const ppItem)
		{
        if(m_cSize)
        {
            *ppItem = m_rg[--m_cSize];
            m_rg[m_cSize] = NULL;
			return TRUE;
        }

        return FALSE;
		}

	inline BOOL Top(void ** const ppItem)
		{
		if(m_cSize)
			{
			*ppItem = m_rg[m_cSize-1];
			return TRUE;
			}
		return FALSE;
		}
	};
	
template<class T> class Vector : public VectorVoid
	{
public:
	inline Vector() : VectorVoid() {}
	inline Vector(const int cSize) : VectorVoid(cSize) {}

	inline T *ElementAt(const int iItem) const
		{
		return (T *) (m_rg[iItem]);
		}	
	};

template<class T> class VectorInt : public VectorVoid
	{
public:
	inline VectorInt() : VectorVoid() {}
	inline VectorInt(const int cSize) : VectorVoid(cSize) {}

	inline T ElementAt(const int iItem) const
		{
		return (T) (m_rg[iItem]);
		}

	inline int AddElement(const T item)
		{
		return VectorVoid::AddElement((void *)item);
		}

	inline int IndexOf(const int pvItem) const
		{
		if (m_rg == NULL)
			return -1;

		for (int i=0; i<m_cSize; i++)
			if ((void *)pvItem == m_rg[i])
				return i;

		return -1;
		}

	inline void RemoveElement(const int pvItem)
		{
		if (!m_rg)
			return;

		const int i = IndexOf(pvItem);
		if (i >= 0)
			RemoveElementAt(i);
		}
	};

typedef Vector<void> VectorPV;
typedef VectorPV * PVectorPV;

template<class T> 
class VecClean : public Vector<T>
	{
public:
	inline VecClean() {};
	inline ~VecClean()
		{
		int cSize = m_cSize;
		while (--cSize >= 0)
			{
			T * const pElem = ElementAt(cSize);
			delete pElem;
			}
		}
	};

#endif
