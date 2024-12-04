// sbgCommonLib headers
#include <sbgCommon.h>

// Local headers
#include "sbgEComSessionInfo.h"

//----------------------------------------------------------------------//
//- Private functions                                                  -//
//----------------------------------------------------------------------//

/*!
 * Reset a session information context.
 *
 * \param[in]   pCtx                        Context.
 */
static void sbgEComSessionInfoCtxReset(SbgEComSessionInfoCtx *pCtx)
{
    assert(pCtx);

    pCtx->string[0] = '\0';

    pCtx->length    = 0;
    pCtx->pageIndex = 0;
    pCtx->nrPages   = 0;
}

//----------------------------------------------------------------------//
//- Public functions                                                   -//
//----------------------------------------------------------------------//

void sbgEComSessionInfoCtxConstruct(SbgEComSessionInfoCtx *pCtx)
{
    assert(pCtx);

    memset(pCtx->string, 0, sizeof(pCtx->string));

    pCtx->length    = 0;
    pCtx->pageIndex = 0;
    pCtx->nrPages   = 0;
}

SbgErrorCode sbgEComSessionInfoCtxProcess(SbgEComSessionInfoCtx *pCtx, uint16_t pageIndex, uint16_t nrPages, const void *pBuffer, size_t size)
{
    SbgErrorCode                         errorCode = SBG_NOT_READY;

    assert(pCtx);
    assert(pageIndex < nrPages);
    assert(pBuffer || (size == 0));

    if (pCtx->pageIndex != pageIndex)
    {
        if ((pageIndex != 0) || (pCtx->pageIndex != pCtx->nrPages))
        {
            SBG_LOG_WARNING(SBG_ERROR, "unexpected page index received, session information context reset");
        }

        sbgEComSessionInfoCtxReset(pCtx);
    }

    if (pageIndex == 0)
    {
        pCtx->nrPages = nrPages;
    }

    if (pCtx->pageIndex == pageIndex)
    {
        size_t                               newSize;

        newSize = pCtx->length + size;

        //
        // Make sure there's at least one available slot for the null-terminating byte.
        //
        if (newSize < sizeof(pCtx->string))
        {
            memcpy(&pCtx->string[pCtx->length], pBuffer, size);
            pCtx->string[newSize] = '\0';

            pCtx->length = newSize;
            pCtx->pageIndex++;

            if (pCtx->pageIndex == pCtx->nrPages)
            {
                errorCode = SBG_NO_ERROR;
            }
        }
        else
        {
            SBG_LOG_ERROR(SBG_BUFFER_OVERFLOW, "session information too large");

            sbgEComSessionInfoCtxReset(pCtx);
        }
    }

    return errorCode;
}

const char *sbgEComSessionInfoCtxGetString(const SbgEComSessionInfoCtx *pCtx)
{
    const char                          *pString = NULL;

    assert(pCtx);

    if ((pCtx->nrPages != 0) && (pCtx->pageIndex == pCtx->nrPages))
    {
        pString = pCtx->string;
    }

    return pString;
}
