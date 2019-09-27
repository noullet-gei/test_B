// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des p�riph�riques.
 Rem : OBLIGATION d'utiliser les d�finitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilit� du code.

 Pour les masques, utiliser �galement les d�finitions propos�e
 Rappel : pour mettre � 1  , reg = reg | Mask (ou Mask est le repr�sente le ou les bits � positionner � 1)
				  pour mettre � 0  , reg = reg&~ Mask (ou Mask est le repr�sente le ou les bits � positionner � 0)
 
*/ 
#include "stm32f103xb.h" 
#include "MyTimer.h"

static void (*IT1_callback)(void);
static void (*IT2_callback)(void);
static void (*IT3_callback)(void);
static void (*IT4_callback)(void);

/**
	* @brief  Active l'horloge et r�gle l'ARR et le PSC du timer vis�
  * @note   Fonction � lancer avant toute autre. Le timer n'est pas encore lanc� (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur � placer dans ARR
	*					int Psc   : valeur � placer dans PSC
  * @retval None
  */
void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc)
{
switch	( (int)Timer )
	{
	case (int)TIM1 : RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; break; 
	case (int)TIM2 : RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; break; 
	case (int)TIM3 : RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; break; 
	case (int)TIM4 : RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; break;
	}
Timer->ARR = Arr;
Timer->PSC = Psc;
}

/**
	* @brief  D�marre le timer consid�r�
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Start(TIM_TypeDef * Timer)
{
Timer->CR1 |= TIM_CR1_CEN;
}


/**
	* @brief  Arr�t le timer consid�r�
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Stop(TIM_TypeDef * Timer)
{
Timer->CR1 &= ~TIM_CR1_CEN;
}


/**
	* @brief  Configure le Timer consid�r� en interruption sur d�bordement.
  * @note   A ce stade, les interruptions ne sont pas valid�s (voir  MyTimer_IT_Enable )
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
	* 				void (*IT_function) (void) : nom (adresse) de la fonction � lancer sur interruption
	*         int Prio : priorit� associ�e � l'interruption
  * @retval None
  */
void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio)
{

switch	( (int)Timer )
	{
	case (int)TIM1 : IT1_callback = IT_function; NVIC->IP[TIM1_UP_IRQn] = Prio; NVIC->ISER[0] = (1 << TIM1_UP_IRQn);  break; 
	case (int)TIM2 : IT2_callback = IT_function; NVIC->IP[TIM2_IRQn] = Prio;    NVIC->ISER[0] = (1 << TIM2_IRQn);   break; 
	case (int)TIM3 : IT3_callback = IT_function; NVIC->IP[TIM3_IRQn] = Prio;    NVIC->ISER[0] = (1 << TIM3_IRQn);   break; 
	case (int)TIM4 : IT4_callback = IT_function; NVIC->IP[TIM4_IRQn] = Prio;    NVIC->ISER[0] = (1 << TIM4_IRQn);   break;
	}
}


/**
	* @brief  Autorise les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Enable(TIM_TypeDef * Timer)
{
Timer->DIER |= TIM_DIER_UIE;
}


/**
	* @brief  Interdit les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_IT_Disable(TIM_TypeDef * Timer)
{
Timer->DIER &= ~TIM_DIER_UIE;
}

void TIM1_UP_IRQHandler(void)
{
TIM1->SR &= ~TIM_SR_UIF;
IT1_callback();
}

void TIM2_IRQHandler(void)
{
TIM2->SR &= ~TIM_SR_UIF;
IT2_callback();
}

void TIM3_IRQHandler(void)
{
TIM3->SR &= ~TIM_SR_UIF;
IT3_callback();
}

void TIM4_IRQHandler(void)
{
TIM4->SR &= ~TIM_SR_UIF;
IT4_callback();
}

