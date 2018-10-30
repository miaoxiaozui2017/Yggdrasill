#ifndef DATABUFFER_HPP
#define DATABUFFER_HPP

#include <iostream>
#include <vector>

template<typename T>
class DataBuffer
{
public:
  DataBuffer(const std::vector<T> & splits);

  bool Charge(std::vector<T> & elems);
  bool Charge(const T * elems, const size_t & len);
  bool Discharge(std::vector<std::vector<T> > & elemVectors);
private:
  bool ChargeElem(const T & elem);

  std::vector<T> m_splitElements;
  size_t m_splitId;

  std::vector<std::vector<T> > m_bufferVector;
  size_t m_chargingVectorId;
  std::vector<size_t> m_chargedIdVector;
  std::vector<size_t> m_dischargedIdVector;
};

template<typename T>
DataBuffer<T>::DataBuffer(const std::vector<T> & splits)
  : m_splitElements(splits)
  , m_chargingVectorId(0)
  , m_splitId(0)
{
  m_bufferVector.resize(1);
}

template<typename T>
bool DataBuffer<T>::Charge(std::vector<T> & elems)
{

  for (size_t i = 0; i < elems.size(); i++)
  {
    ChargeElem(elems[i]);
  }
  return true;
}

template<typename T>
bool DataBuffer<T>::Charge(const T * elems, const size_t & len)
{
  for (size_t i = 0; i < len; i++)
  {
    ChargeElem(elems[i]);
  }
  return true;
}

template<typename T>
bool DataBuffer<T>::ChargeElem(const T & elem)
{
  m_bufferVector[m_chargingVectorId].push_back(elem);

  if (elem == m_splitElements[m_splitId])
  {
    m_splitId++;
    if (m_splitId == m_splitElements.size())
    {
      m_splitId = 0;
      m_chargedIdVector.push_back(m_chargingVectorId);
      if (m_dischargedIdVector.empty())
      {
        m_bufferVector.push_back(std::vector<T>());
        m_chargingVectorId = m_bufferVector.size() - 1;
      }
      else
      {
        m_chargingVectorId = *(m_dischargedIdVector.end() - 1);
        m_dischargedIdVector.pop_back();
        m_bufferVector[m_chargingVectorId].clear();
      }
    }
  }
  else
  {
    m_splitId = 0;
  }
  return true;
}

template<typename T>
bool DataBuffer<T>::Discharge(std::vector<std::vector<T> > & elemVectors)
{
  elemVectors.clear();
  for (std::vector<size_t>::iterator it = m_chargedIdVector.begin(); it != m_chargedIdVector.end(); it++)
  {
    elemVectors.push_back(m_bufferVector[*it]);
  }
  m_dischargedIdVector.insert(m_dischargedIdVector.end(), m_chargedIdVector.begin(), m_chargedIdVector.end());
  m_chargedIdVector.clear();
  return true;
}

#endif